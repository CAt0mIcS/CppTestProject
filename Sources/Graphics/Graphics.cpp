#include "Graphics.h"
#include "../Utils/RLogger.h"
#include "../Utils/RException.h"
#include "../Utils/RAssert.h"

#include "Pipeline/Shader.h"

#include <vulkan/vulkan.h>


namespace At0::VulkanTesting
{
	Graphics::Graphics()
	{
		s_Instance = this;

		m_Instance = std::make_unique<VulkanInstance>();
		m_Surface = std::make_unique<Surface>();
		m_PhysicalDevice = std::make_unique<PhysicalDevice>();
		m_LogicalDevice = std::make_unique<LogicalDevice>();
		m_Swapchain = std::make_unique<Swapchain>();

		CreateGraphicsPipeline();
		CreateFramebuffers();
		CreateCommandPoolAndBuffers();
		CreateSyncObjects();
	}

	void Graphics::CreateGraphicsPipeline()
	{
		Attachment attachment;

		std::vector<VkAttachmentReference> attachmentReferences;
		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachmentReferences.emplace_back(std::move(colorAttachmentRef));

		Subpass subpass(attachmentReferences);

		m_Renderpass = std::make_unique<Renderpass>(
			std::vector<VkAttachmentDescription>{ attachment.GetDescription() },
			std::vector<VkSubpassDescription>{ subpass.GetDescription() });
		Shader shader("Resources/Shaders/VertexShader.spv", "Resources/Shaders/FragmentShader.spv");

		m_GraphicsPipeline = std::make_unique<GraphicsPipeline>(std::move(shader), *m_Renderpass);
	}

	void Graphics::CreateFramebuffers()
	{
		m_Framebuffers.resize(m_Swapchain->GetNumberOfImages());
		for (uint32_t i = 0; i < m_Swapchain->GetNumberOfImages(); ++i)
		{
			m_Framebuffers[i] = { { m_Swapchain->GetImageView(i) } };
		}
	}

	void Graphics::CreateCommandPoolAndBuffers()
	{
		m_CommandPool = std::make_unique<CommandPool>();
		m_CommandBuffers.resize(m_Framebuffers.size());

		uint32_t i = 0;
		for (std::unique_ptr<CommandBuffer>& cmdBuff : m_CommandBuffers)
		{
			cmdBuff = std::make_unique<CommandBuffer>();

			// Prerecorded commands
			cmdBuff->Begin();

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = *m_Renderpass;
			renderPassInfo.framebuffer = m_Framebuffers[i];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = m_Swapchain->GetExtent();
			VkClearValue clearColor{ 0.0f, 0.0f, 0.0f, 1.0f };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;
			vkCmdBeginRenderPass(*cmdBuff, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(*cmdBuff, VK_PIPELINE_BIND_POINT_GRAPHICS, *m_GraphicsPipeline);
			vkCmdDraw(*cmdBuff, 3, 1, 0, 0);

			vkCmdEndRenderPass(*cmdBuff);

			cmdBuff->End();
			++i;
		}
	}

	void Graphics::CreateSyncObjects()
	{
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
		vkDeviceWaitIdle(*m_LogicalDevice);

		// not production level code here...
		if (s_Instance)
		{
			delete s_Instance;
			s_Instance = nullptr;
		}

		for (uint32_t i = 0; i < s_MaxFramesInFlight; ++i)
		{
			vkDestroySemaphore(*m_LogicalDevice, m_ImageAvailableSemaphore[i], nullptr);
			vkDestroySemaphore(*m_LogicalDevice, m_RenderFinishedSemaphore[i], nullptr);
			vkDestroyFence(*m_LogicalDevice, m_InFlightFences[i], nullptr);
		}

		m_Framebuffers.clear();
		m_GraphicsPipeline.reset();
		m_Renderpass.reset();
		m_Swapchain.reset();
		m_LogicalDevice.reset();
		m_Surface.reset();
		m_Instance.reset();
	}

	Graphics& Graphics::Get()
	{
		if (!s_Instance)
			new Graphics();

		return *s_Instance;
	}

	void Graphics::Update()
	{
		// Wait for fence in vkQueueSubmit to signal,
		// which means that the command buffer finished executing
		vkWaitForFences(
			*m_LogicalDevice, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);
		vkResetFences(*m_LogicalDevice, 1, &m_InFlightFences[m_CurrentFrame]);

		uint32_t imageIndex;
		vkAcquireNextImageKHR(*m_LogicalDevice, *m_Swapchain, UINT64_MAX,
			m_ImageAvailableSemaphore[m_CurrentFrame], VK_NULL_HANDLE, &imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &m_ImageAvailableSemaphore[m_CurrentFrame];
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;

		VkCommandBuffer buffer = *m_CommandBuffers[imageIndex].get();
		submitInfo.pCommandBuffers = &buffer;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &m_RenderFinishedSemaphore[m_CurrentFrame];

		// Fence will be signaled once the command buffer finishes executing
		RAY_VK_THROW_FAILED(vkQueueSubmit(m_LogicalDevice->GetGraphicsQueue(), 1, &submitInfo,
								m_InFlightFences[m_CurrentFrame]),
			"Failed to submit to queue.");

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &m_RenderFinishedSemaphore[m_CurrentFrame];

		VkSwapchainKHR swapchain = *m_Swapchain.get();
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &swapchain;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;	 // Optional
		vkQueuePresentKHR(m_LogicalDevice->GetPresentQueue(), &presentInfo);

		// Loop around every time s_MaxFramesInFlight is surpassed
		m_CurrentFrame = (m_CurrentFrame + 1) % s_MaxFramesInFlight;
	}
}  // namespace At0::VulkanTesting
