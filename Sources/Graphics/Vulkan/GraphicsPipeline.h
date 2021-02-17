#pragma once

#include <vulkan/vulkan_core.h>
#include <string_view>
#include <vector>

#include "Pipeline.h"

#include "Bindable.h"
#include "Shader.h"


namespace At0::VulkanTesting
{
	class Renderpass;
	class CommandBuffer;

	class GraphicsPipeline : public Pipeline
	{
	public:
		GraphicsPipeline(const Renderpass& renderpass, std::string_view vShaderFilepath,
			std::string_view fShaderFilepath, std::vector<Shader::Define> defines = {});
		~GraphicsPipeline();

		static std::string GetUID(const Renderpass& renderpass, std::string_view vShaderFilepath,
			std::string_view fShaderFilepath);

		operator const VkPipeline&() const override { return m_Pipeline; }
		const VkPipelineLayout& GetLayout() const override { return m_Layout; }
		const VkDescriptorSetLayout& GetDescriptorSetLayout() const override
		{
			return m_DescriptorSetLayout;
		}
		const VkDescriptorPool& GetDescriptorPool() const override { return m_DescriptorPool; }

		VkPipelineBindPoint GetBindPoint() const override
		{
			return VK_PIPELINE_BIND_POINT_GRAPHICS;
		}

	private:
		void CreateShaderProgram(const std::vector<std::string_view>& filepaths);
		void CreateDescriptorSetLayout();
		void CreateDescriptorPool();
		void CreatePipelineLayout();
		void CreatePipeline(const Renderpass& renderpass);

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
