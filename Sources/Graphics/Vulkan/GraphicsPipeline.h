#pragma once

#include <vulkan/vulkan_core.h>
#include <string_view>
#include <vector>

#include "Base.h"

#include "Pipeline.h"
#include "Shader.h"
#include "Vertex.h"


namespace At0::VulkanTesting
{
	class Renderpass;
	class CommandBuffer;

	class GraphicsPipeline : public Pipeline
	{
	public:
		GraphicsPipeline(const VertexLayout& vertexLayout,
			const std::vector<std::string_view>& shaders, std::vector<Shader::Define> defines = {});
		~GraphicsPipeline();

		static std::string GetUID(const VertexLayout& vertexLayout,
			const std::vector<std::string_view>& shaders, std::vector<Shader::Define> defines = {});

		operator const VkPipeline&() const override { return m_Pipeline; }
		const VkPipelineLayout& GetLayout() const override { return m_Layout; }
		const VkDescriptorSetLayout& GetDescriptorSetLayout() const override
		{
			return m_DescriptorSetLayout;
		}
		const VkDescriptorPool& GetDescriptorPool() const override { return m_DescriptorPool; }
		const Shader& GetShader() const override { return m_Shader; }

		VkPipelineBindPoint GetBindPoint() const override
		{
			return VK_PIPELINE_BIND_POINT_GRAPHICS;
		}

	private:
		void CreateShaderProgram(const std::vector<std::string_view>& filepaths);
		void CreateDescriptorSetLayout();
		void CreateDescriptorPool();
		void CreatePipelineLayout();
		void CreatePipeline(const VertexLayout& vertexLayout);

	private:
		VkPipeline m_Pipeline;
		VkPipelineLayout m_Layout;

		VkDescriptorSetLayout m_DescriptorSetLayout;
		VkDescriptorPool m_DescriptorPool;

		Shader m_Shader;

		std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;
		std::vector<Shader::Define> m_Defines;
	};
}  // namespace At0::VulkanTesting
