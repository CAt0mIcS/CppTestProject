﻿#include "Graphics.h"
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

	Graphics::~Graphics()
	{
		// not production level code here...
		if (s_Instance)
		{
			delete s_Instance;
			s_Instance = nullptr;
		}

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
}  // namespace At0::VulkanTesting
