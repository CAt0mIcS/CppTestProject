#pragma once

#include <vulkan/vulkan_core.h>
#include <string_view>
#include <vector>

#include "Bindable.h"
#include "Shader.h"


namespace At0::VulkanTesting
{
	class Renderpass;
	class CommandBuffer;

	class GraphicsPipeline : public Bindable
	{
	public:
		GraphicsPipeline(const Renderpass& renderpass, std::string_view vShaderFilepath,
			std::string_view fShaderFilepath);
		~GraphicsPipeline();

		void Bind(CommandBuffer& cmdBuff) override;

		static std::string GetUID(const Renderpass& renderpass, std::string_view vShaderFilepath,
			std::string_view fShaderFilepath);

		operator const VkPipeline&() const { return m_Pipeline; }
		const VkPipelineLayout& GetLayout() const { return m_Layout; }

		const VkDescriptorSetLayout& GetDescriptorSetLayout() const
		{
			return m_DescriptorSetLayout;
		}
		const VkDescriptorPool& GetDescriptorPool() const { return m_DescriptorPool; }

	private:
		static VkShaderModule CreateShader(std::vector<char> src);
		static std::vector<char> ReadShader(std::string_view filepath);

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

		std::vector<VkShaderModule> m_ShaderModules;
		std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;
	};
}  // namespace At0::VulkanTesting
