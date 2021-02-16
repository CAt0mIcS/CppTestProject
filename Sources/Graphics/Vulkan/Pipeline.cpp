#include "Pipeline.h"
#include "Graphics/Graphics.h"
#include "Window.h"

#include "VertexBuffer.h"

#include <fstream>
#include <sstream>


namespace At0::VulkanTesting
{
	GraphicsPipeline::GraphicsPipeline(const Renderpass& renderpass,
		std::string_view vShaderFilepath, std::string_view fShaderFilepath)
	{
		CreateShaderProgram({ vShaderFilepath, fShaderFilepath });
		CreateDescriptorSetLayout();
		CreateDescriptorPool();
		CreatePipelineLayout();
		CreatePipeline(renderpass);
	}

	GraphicsPipeline::~GraphicsPipeline()
	{
		vkDestroyPipeline(Graphics::Get().GetLogicalDevice(), m_Pipeline, nullptr);
		vkDestroyPipelineLayout(Graphics::Get().GetLogicalDevice(), m_Layout, nullptr);

		vkDestroyDescriptorSetLayout(
			Graphics::Get().GetLogicalDevice(), m_DescriptorSetLayout, nullptr);
		vkDestroyDescriptorPool(Graphics::Get().GetLogicalDevice(), m_DescriptorPool, nullptr);
	}

	void GraphicsPipeline::Bind(CommandBuffer& cmdBuff)
	{
		vkCmdBindPipeline(cmdBuff, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline);
	}

	std::string GraphicsPipeline::GetUID(const Renderpass& renderpass,
		std::string_view vShaderFilepath, std::string_view fShaderFilepath)
	{
		using namespace std::string_literals;

		// Only fill oss once
		static std::unique_ptr<std::ostringstream> oss;
		if (!oss)
		{
			oss = std::make_unique<std::ostringstream>();
			*oss << typeid(GraphicsPipeline).name() << "#" << (VkRenderPass)renderpass << "#"
				 << vShaderFilepath << "#" << fShaderFilepath;
		}

		return oss->str();
	}

	VkShaderModule GraphicsPipeline::CreateShader(std::vector<char> src)
	{
		VkShaderModule shaderModule;

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = src.size();
		createInfo.pCode = reinterpret_cast<uint32_t*>(src.data());

		RAY_VK_THROW_FAILED(vkCreateShaderModule(Graphics::Get().GetLogicalDevice(), &createInfo,
								nullptr, &shaderModule),
			"Failed to create fragment shader module.");

		return shaderModule;
	}

	std::vector<char> GraphicsPipeline::ReadShader(std::string_view filepath)
	{
		std::ifstream reader(filepath.data(), std::ios::ate | std::ios::binary);

		size_t filesize = (size_t)reader.tellg();
		RAY_MEXPECTS(filesize != 0 && filesize != SIZE_MAX, "Shader file not found");
		std::vector<char> code(filesize);

		reader.seekg(0);
		reader.read(code.data(), filesize);
		reader.close();

		return code;
	}

	void GraphicsPipeline::CreateShaderProgram(const std::vector<std::string_view>& filepaths)
	{
		for (std::string_view filepath : filepaths)
		{
			std::vector<char> code = ReadShader(filepath);
			VkShaderStageFlagBits stageFlag = Shader::GetShaderStage(filepath);
			VkShaderModule shaderModule = CreateShader(code);

			VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {};
			pipelineShaderStageCreateInfo.sType =
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			pipelineShaderStageCreateInfo.stage = stageFlag;
			pipelineShaderStageCreateInfo.module = shaderModule;
			pipelineShaderStageCreateInfo.pName = "main";
			m_ShaderStages.emplace_back(pipelineShaderStageCreateInfo);
			m_ShaderModules.emplace_back(shaderModule);
		}

		// m_Shader.CreateReflection();
	}

	void GraphicsPipeline::CreateDescriptorSetLayout()
	{
		const std::vector<VkDescriptorSetLayoutBinding>& descriptorSetLayoutBindings =
			m_Shader.GetDescriptorSetLayoutBindings();

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
			m_Shader.GetDescriptorPoolSizes();

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
		// ---------------------------------------------------------------------------------------
		// Pipeline Layout
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = &m_DescriptorSetLayout;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		RAY_VK_THROW_FAILED(vkCreatePipelineLayout(Graphics::Get().GetLogicalDevice(),
								&pipelineLayoutInfo, nullptr, &m_Layout),
			"Failed to create pipeline layout.");
	}

	void GraphicsPipeline::CreatePipeline(const Renderpass& renderpass)
	{
		// ---------------------------------------------------------------------------------------
		// Vertex Input
		auto bindingDesc = Vertex::GetBindingDescription();
		auto attribDesc = Vertex::GetAttributeDescriptions(0, 1);

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
		pipelineInfo.renderPass = renderpass;
		pipelineInfo.subpass = 0;  // Index of subpass used with pipeline

		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;

		RAY_VK_THROW_FAILED(vkCreateGraphicsPipelines(Graphics::Get().GetLogicalDevice(),
								VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline),
			"Failed to create graphics pipeline.");

		// Destroy shader modules as they aren't needed anymore
		for (auto shaderModule : m_ShaderModules)
		{
			vkDestroyShaderModule(Graphics::Get().GetLogicalDevice(), shaderModule, nullptr);
		}

		// These vectors aren't needed anymore and can be freed
		m_ShaderModules.resize(0);
		m_ShaderStages.resize(0);
	}

}  // namespace At0::VulkanTesting
