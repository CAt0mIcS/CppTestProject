#include "pch.h"
#include "GraphicsPipeline.h"

#include "Graphics/Graphics.h"
#include "LogicalDevice.h"
#include "Renderpass/Renderpass.h"
#include "Window.h"

#include "VertexBuffer.h"

#include <glslang/SPIRV/GlslangToSpv.h>


namespace At0::VulkanTesting
{
	GraphicsPipeline::GraphicsPipeline(const VertexLayout& vertexLayout,
		const std::vector<std::string_view>& shaders, std::vector<Shader::Define> defines)
		: m_Defines(std::move(defines))
	{
		static bool glslangInitialized = false;
		if (!glslangInitialized)
		{
			glslang::InitializeProcess();
			glslangInitialized = true;
		}

		CreateShaderProgram(shaders);
		CreateDescriptorSetLayout();
		CreateDescriptorPool();
		CreatePipelineLayout();
		CreatePipeline(vertexLayout);
	}

	GraphicsPipeline::~GraphicsPipeline()
	{
		vkDestroyPipeline(Graphics::Get().GetLogicalDevice(), m_Pipeline, nullptr);
		vkDestroyPipelineLayout(Graphics::Get().GetLogicalDevice(), m_Layout, nullptr);

		vkDestroyDescriptorSetLayout(
			Graphics::Get().GetLogicalDevice(), m_DescriptorSetLayout, nullptr);
		vkDestroyDescriptorPool(Graphics::Get().GetLogicalDevice(), m_DescriptorPool, nullptr);
	}

	std::string GraphicsPipeline::GetUID(const VertexLayout& vertexLayout,
		const std::vector<std::string_view>& shaders, std::vector<Shader::Define> defines)
	{
		using namespace std::string_literals;

		// Only fill oss once
		static Scope<std::ostringstream> oss;
		if (!oss)
		{
			oss = MakeScope<std::ostringstream>();
			*oss << typeid(GraphicsPipeline).name();

			for (std::string_view filepath : shaders)
				*oss << "#" << filepath;
		}

		return oss->str();
	}

	void GraphicsPipeline::CreateShaderProgram(const std::vector<std::string_view>& filepaths)
	{
		std::ostringstream defineBlock;
		for (const auto& [defineName, defineValue] : m_Defines)
			defineBlock << "#define " << defineName << " " << defineValue << '\n';

		for (std::string_view shaderStage : filepaths)
		{
			auto fileLoaded = Shader::ReadFile(shaderStage);

			if (!fileLoaded)
				RAY_THROW_RUNTIME("Unable to create pipeline because of missing m_Shader stage");

			VkShaderStageFlagBits stageFlag = Shader::GetShaderStage(shaderStage);
			VkShaderModule shaderModule =
				m_Shader.CreateShaderModule(shaderStage, *fileLoaded, defineBlock.str(), stageFlag);

			VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo{};
			pipelineShaderStageCreateInfo.sType =
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			pipelineShaderStageCreateInfo.stage = stageFlag;
			pipelineShaderStageCreateInfo.module = shaderModule;
			pipelineShaderStageCreateInfo.pName = "main";
			m_ShaderStages.emplace_back(pipelineShaderStageCreateInfo);
		}

		m_Shader.CreateReflection();
	}

	void GraphicsPipeline::CreateDescriptorSetLayout()
	{
		const std::vector<VkDescriptorSetLayoutBinding>& descriptorSetLayoutBindings =
			m_Shader.GetDescriptorSetLayouts();

		VkDescriptorSetLayoutCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		createInfo.flags =
			/*m_PushDescriptors ? VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR : */ 0;
		createInfo.bindingCount = (uint32_t)descriptorSetLayoutBindings.size();
		createInfo.pBindings = descriptorSetLayoutBindings.data();

		RAY_VK_THROW_FAILED(vkCreateDescriptorSetLayout(Graphics::Get().GetLogicalDevice(),
								&createInfo, nullptr, &m_DescriptorSetLayout),
			"Failed to create descriptor set layout.");
	}

	void GraphicsPipeline::CreateDescriptorPool()
	{
		const std::vector<VkDescriptorPoolSize>& descriptorPoolSizes =
			m_Shader.GetDescriptorPools();

		VkDescriptorPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		createInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		createInfo.maxSets = 8192;
		createInfo.poolSizeCount = (uint32_t)descriptorPoolSizes.size();
		createInfo.pPoolSizes = descriptorPoolSizes.data();

		RAY_VK_THROW_FAILED(vkCreateDescriptorPool(Graphics::Get().GetLogicalDevice(), &createInfo,
								nullptr, &m_DescriptorPool),
			"Failed to create descriptor pool");
	}

	void GraphicsPipeline::CreatePipelineLayout()
	{
		auto pushConstantRanges = m_Shader.GetPushConstantRanges();

		// ---------------------------------------------------------------------------------------
		// Pipeline Layout
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = &m_DescriptorSetLayout;
		pipelineLayoutInfo.pushConstantRangeCount = (uint32_t)pushConstantRanges.size();
		pipelineLayoutInfo.pPushConstantRanges = pushConstantRanges.data();

		RAY_VK_THROW_FAILED(vkCreatePipelineLayout(Graphics::Get().GetLogicalDevice(),
								&pipelineLayoutInfo, nullptr, &m_Layout),
			"Failed to create pipeline layout.");
	}

	void GraphicsPipeline::CreatePipeline(const VertexLayout& vertexLayout)
	{
		// ---------------------------------------------------------------------------------------
		// Vertex Input
		auto bindingDesc = vertexLayout.GetBindingDescription();
		auto attribDesc = vertexLayout.GetAttributeDescriptions();

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &bindingDesc;
		vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)attribDesc.size();
		vertexInputInfo.pVertexAttributeDescriptions = attribDesc.data();

		// ---------------------------------------------------------------------------------------
		// Input Assembler
		VkPipelineInputAssemblyStateCreateInfo inputAssembler{};
		inputAssembler.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembler.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembler.primitiveRestartEnable = VK_FALSE;

		// ---------------------------------------------------------------------------------------
		// Viewport State
		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = nullptr;
		viewportState.scissorCount = 1;
		viewportState.pScissors = nullptr;

		// ---------------------------------------------------------------------------------------
		// Rasterizer
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f;
		rasterizer.depthBiasClamp = 0.0f;
		rasterizer.depthBiasSlopeFactor = 0.0f;

		// ---------------------------------------------------------------------------------------
		// Multisampling
		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f;
		multisampling.pSampleMask = nullptr;
		multisampling.alphaToCoverageEnable = VK_FALSE;
		multisampling.alphaToOneEnable = VK_FALSE;

		// ---------------------------------------------------------------------------------------
		// Color Blending
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
											  VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_TRUE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		// ---------------------------------------------------------------------------------------
		// Dynamic state
		VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
		dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateInfo.dynamicStateCount = std::size(dynamicStates);
		dynamicStateInfo.pDynamicStates = dynamicStates;

		// ---------------------------------------------------------------------------------------
		// Pipeline
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = (uint32_t)m_ShaderStages.size();
		pipelineInfo.pStages = m_ShaderStages.data();
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembler;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicStateInfo;
		pipelineInfo.layout = m_Layout;
		pipelineInfo.renderPass = Graphics::Get().GetRenderpass();
		pipelineInfo.subpass = 0;  // Index of subpass used with pipeline

		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;

		RAY_VK_THROW_FAILED(vkCreateGraphicsPipelines(Graphics::Get().GetLogicalDevice(),
								VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline),
			"Failed to create graphics pipeline.");

		// Destroy shader modules as they aren't needed anymore
		for (const VkPipelineShaderStageCreateInfo& shaderStage : m_ShaderStages)
		{
			vkDestroyShaderModule(Graphics::Get().GetLogicalDevice(), shaderStage.module, nullptr);
		}

		// These vectors aren't needed anymore and can be freed
		m_ShaderStages.resize(0);
	}

}  // namespace At0::VulkanTesting
