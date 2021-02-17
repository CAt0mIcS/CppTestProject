#include "pch.h"
#include "Graphics/Graphics.h"
#include "Utils/RLogger.h"
#include "Utils/RException.h"
#include "Utils/RAssert.h"
#include "Window.h"

#include <chrono>

#include "Vulkan/Swapchain.h"
#include "Vulkan/VulkanInstance.h"
#include "Vulkan/Surface.h"
#include "Vulkan/PhysicalDevice.h"
#include "Vulkan/LogicalDevice.h"
#include "Vulkan/Framebuffer.h"
#include "Vulkan/Renderpass/Renderpass.h"
#include "Vulkan/Renderpass/Attachment.h"
#include "Vulkan/Renderpass/Subpass.h"
#include "Vulkan/Commands/CommandPool.h"
#include "Vulkan/Commands/CommandBuffer.h"

#include "Core/Codex.h"

#include "Primitives/Cube.h"
#include "Primitives/Square.h"
#include "Primitives/Triangle.h"
#include "Primitives/Model.h"
#include "Primitives/Mesh.h"


namespace At0::VulkanTesting
{
	Graphics::Graphics()
	{
		s_Instance = this;

		UpdateViewport();
		UpdateScissor();

		m_Instance = MakeScope<VulkanInstance>();
		m_Surface = MakeScope<Surface>();
		m_PhysicalDevice = MakeScope<PhysicalDevice>();
		m_LogicalDevice = MakeScope<LogicalDevice>();
		LoadExtensionFunctions();

		m_Swapchain = MakeScope<Swapchain>();

		CreateRenderpass();
		CreateFramebuffers();

		m_CommandPool = MakeScope<CommandPool>();

		// --------------------------------------------------------------
		// Create all drawables
		CreateDrawables();

		CreateCommandBuffers();
		CreateSyncObjects();

		int width, height;
		Window::Get().GetFramebufferSize(&width, &height);

		SceneCamera.SetPosition(glm::vec3(0.0f, 0.0f, -2.5f));
		SceneCamera.SetRotation(glm::vec3(0.0f));
		SceneCamera.SetPerspective(60.0f, (float)width / (float)height, 0.1f, 256.0f);
		SceneCamera.SetMovementSpeed(2.0f);
	}

	void Graphics::LoadExtensionFunctions() {}

	void Graphics::CreateRenderpass()
	{
		Attachment attachment;

		std::vector<VkAttachmentReference> attachmentReferences;
		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachmentReferences.emplace_back(std::move(colorAttachmentRef));

		Subpass subpass(attachmentReferences);

		m_Renderpass = MakeScope<Renderpass>(
			std::vector<VkAttachmentDescription>{ attachment.GetDescription() },
			std::vector<VkSubpassDescription>{ subpass.GetDescription() });
	}

	void Graphics::CreateFramebuffers()
	{
		m_Framebuffers.resize(m_Swapchain->GetNumberOfImages());
		for (uint32_t i = 0; i < m_Swapchain->GetNumberOfImages(); ++i)
		{
			m_Framebuffers[i] =
				MakeScope<Framebuffer>(std::vector<VkImageView>{ m_Swapchain->GetImageView(i) });
		}
	}

	void Graphics::CreateDrawables()
	{
		m_Model = MakeScope<Model>("Resources/Models/Nanosuit/nanosuit.obj");
		m_Model2 = MakeScope<Model>("Resources/Models/Nanosuit/nanosuit.obj");

		m_Model2->Translate(glm::vec3(9.0f, 0.0f, 0.0f));

		m_Drawables.emplace_back(MakeScope<Cube>());
		m_Drawables.emplace_back(MakeScope<Triangle>());

		TransformComponent& triangleTransform =
			m_Drawables.back()->GetEntity().Get<TransformComponent>();
		triangleTransform.Translation = { 1.0f, 1.0f, 0.0f };

		m_Drawables.emplace_back(MakeScope<Square>());

		TransformComponent& squareTransform =
			m_Drawables.back()->GetEntity().Get<TransformComponent>();
		squareTransform.Translation = { -1.0f, 1.0f, 0.0f };
	}

	void Graphics::CreateCommandBuffers()
	{
		m_CommandBuffers.resize(m_Framebuffers.size());

		for (uint32_t i = 0; i < m_CommandBuffers.size(); ++i)
		{
			m_CommandBuffers[i] = MakeScope<CommandBuffer>();

			// Prerecord commands
			RecordCommandBuffer(*m_CommandBuffers[i], *m_Framebuffers[i]);
		}
	}

	void Graphics::RecordCommandBuffer(CommandBuffer& cmdBuff, const Framebuffer& framebuffer)
	{
		cmdBuff.Begin();

		VkClearValue clearColor{ 0.0137254f, 0.014117f, 0.0149019f };
		m_Renderpass->Begin(cmdBuff, framebuffer, clearColor);

		const VkViewport viewports[] = { m_Viewport };
		const VkRect2D scissors[] = { m_Scissor };
		vkCmdSetViewport(cmdBuff, 0, std::size(viewports), viewports);
		vkCmdSetScissor(cmdBuff, 0, std::size(scissors), scissors);

		m_Model->CmdDraw(cmdBuff);
		m_Model2->CmdDraw(cmdBuff);
		for (Scope<Drawable>& drawable : m_Drawables)
		{
			drawable->CmdDraw(cmdBuff);
		}

		m_Renderpass->End(cmdBuff);

		cmdBuff.End();
	}

	void Graphics::CreateSyncObjects()
	{
		m_ImagesInFlight.resize(m_Swapchain->GetNumberOfImages(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreCreateInfo{};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (uint32_t i = 0; i < s_MaxFramesInFlight; ++i)
		{
			RAY_VK_THROW_FAILED(vkCreateSemaphore(*m_LogicalDevice, &semaphoreCreateInfo, nullptr,
									&m_ImageAvailableSemaphore[i]),
				"Failed to create semaphore.");

			RAY_VK_THROW_FAILED(vkCreateSemaphore(*m_LogicalDevice, &semaphoreCreateInfo, nullptr,
									&m_RenderFinishedSemaphore[i]),
				"Failed to create semaphore.");

			RAY_VK_THROW_FAILED(
				vkCreateFence(*m_LogicalDevice, &fenceCreateInfo, nullptr, &m_InFlightFences[i]),
				"Failed to create fence.");
		}
	}

	Graphics::~Graphics()
	{
		Log::Debug("Destroying Graphics object...");
		vkDeviceWaitIdle(*m_LogicalDevice);

		for (uint32_t i = 0; i < s_MaxFramesInFlight; ++i)
		{
			vkDestroySemaphore(*m_LogicalDevice, m_ImageAvailableSemaphore[i], nullptr);
			vkDestroySemaphore(*m_LogicalDevice, m_RenderFinishedSemaphore[i], nullptr);
			vkDestroyFence(*m_LogicalDevice, m_InFlightFences[i], nullptr);
		}

		m_CommandBuffers.clear();

		// ---------------------------------------
		// Reset all drawables here
		m_Drawables.clear();
		m_Model.reset();
		m_Model2.reset();

		// Explicit Codex destruction because Drawables might still need resources
		// even if the ref count of the resource is 1
		Codex::Shutdown();

		m_CommandPool.reset();
		m_Framebuffers.clear();
		m_Renderpass.reset();

		m_Swapchain.reset();
		m_LogicalDevice.reset();
		m_Surface.reset();
		m_Instance.reset();

		Log::Debug("Destroyed Graphics object");
	}

	Graphics& Graphics::Get() { return *s_Instance; }

	void Graphics::Create()
	{
		if (!s_Instance)
			new Graphics();
	}

	void Graphics::Destroy()
	{
		if (s_Instance)
		{
			delete s_Instance;
			s_Instance = nullptr;
		}
	}

	void Graphics::Update(float dt)
	{
		m_Dt = dt;
		SceneCamera.SetRotationSpeed(260.0f * dt);
		SceneCamera.Update(dt);

		// Wait for fence in vkQueueSubmit to become signaled,
		// which means that the command buffer finished executing
		vkWaitForFences(
			*m_LogicalDevice, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);

		// Index representing the index of the next image to draw (0-2 in this case)
		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(*m_LogicalDevice, *m_Swapchain, UINT64_MAX,
			m_ImageAvailableSemaphore[m_CurrentFrame],	//  Signalled by this function when an image
														//  is acquired
			VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapchain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			RAY_THROW_RUNTIME("Failed to acquire next swapchain image.");

		// VK_TODO: We might need more than one descriptor/uniform buffer per drawable to account
		// for asynchronous image rendering
		UpdateDrawables();

		// Check if a previous frame is using this image (i.e. there is its fence to wait on)
		if (m_ImagesInFlight[imageIndex] != VK_NULL_HANDLE)
			vkWaitForFences(
				*m_LogicalDevice, 1, &m_ImagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);

		// Mark the image as now being in use by this frame
		m_ImagesInFlight[imageIndex] = m_InFlightFences[m_CurrentFrame];

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores =
			&m_ImageAvailableSemaphore[m_CurrentFrame];	 // Wait until image was acquired
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;

		VkCommandBuffer buffer = *m_CommandBuffers[imageIndex].get();
		submitInfo.pCommandBuffers = &buffer;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores =
			&m_RenderFinishedSemaphore[m_CurrentFrame];	 // Signal when rendering finished and
														 // presentation can happen

		vkResetFences(*m_LogicalDevice, 1, &m_InFlightFences[m_CurrentFrame]);

		// Fence will be signaled once the command buffer finishes executing
		RAY_VK_THROW_FAILED(vkQueueSubmit(m_LogicalDevice->GetGraphicsQueue(), 1, &submitInfo,
								m_InFlightFences[m_CurrentFrame]),
			"Failed to submit to queue.");

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores =
			&m_RenderFinishedSemaphore[m_CurrentFrame];	 // Wait for rendering to finish so that
														 // presentation can happen

		VkSwapchainKHR swapchain = *m_Swapchain.get();
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &swapchain;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;	 // Optional
		result = vkQueuePresentKHR(m_LogicalDevice->GetPresentQueue(), &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
			m_FramebufferResized)
			RecreateSwapchain();
		else if (result != VK_SUCCESS)
			RAY_THROW_RUNTIME("Failed to present swap chain image!");

		// Loop around every time s_MaxFramesInFlight is surpassed
		// (clamped between 0 and 1 in this case)
		m_CurrentFrame = (m_CurrentFrame + 1) % s_MaxFramesInFlight;
	}

	void Graphics::RecreateSwapchain()
	{
		// Minimized window will have framebuffer size of [0 | 0]
		// Sleep until window is back in foreground
		int width, height;
		Window::Get().GetFramebufferSize(&width, &height);
		while (width == 0 || height == 0)
		{
			Window::Get().GetFramebufferSize(&width, &height);
			Window::Get().WaitForEvents();
		}

		vkDeviceWaitIdle(*m_LogicalDevice);

		m_CommandBuffers.clear();
		m_CommandPool.reset();
		m_Framebuffers.clear();

		m_Renderpass.reset();

		m_Swapchain.reset();

		m_Swapchain = MakeScope<Swapchain>(m_Swapchain.get());
		CreateRenderpass();
		UpdateViewport();
		UpdateScissor();
		CreateFramebuffers();
		m_CommandPool = MakeScope<CommandPool>();
		CreateCommandBuffers();

		Window::Get().GetFramebufferSize(&width, &height);
		SceneCamera.UpdateAspectRatio((float)width / (float)height);

		m_FramebufferResized = false;
	}

	void Graphics::UpdateViewport()
	{
		int width, height;
		Window::Get().GetFramebufferSize(&width, &height);

		m_Viewport.x = 0.0f;
		m_Viewport.y = 0.0f;
		m_Viewport.width = (float)width;
		m_Viewport.height = (float)height;
		m_Viewport.minDepth = 0.0f;
		m_Viewport.maxDepth = 1.0f;
	}

	void Graphics::UpdateScissor()
	{
		int width, height;
		Window::Get().GetFramebufferSize(&width, &height);

		m_Scissor.offset = { 0, 0 };
		m_Scissor.extent = { (uint32_t)width, (uint32_t)height };
	}

	void Graphics::UpdateDrawables()
	{
		m_Model->Update();
		m_Model2->Update();
		for (Scope<Drawable>& drawable : m_Drawables)
		{
			drawable->Update();
		}
	}
}  // namespace At0::VulkanTesting
