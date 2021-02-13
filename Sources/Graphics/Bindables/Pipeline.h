#pragma once

#include <vulkan/vulkan_core.h>
#include <string_view>
#include <vector>


namespace At0::VulkanTesting
{
	class Renderpass;

	class GraphicsPipeline
	{
	public:
		GraphicsPipeline(const Renderpass& renderpass, std::string_view vShaderFilepath,
			std::string_view fShaderFilepath);
		~GraphicsPipeline();

		operator const VkPipeline&() const { return m_Pipeline; }
		const VkPipelineLayout& GetLayout() const { return m_Layout; }

	private:
		static VkShaderModule CreateShader(std::vector<char> src);
		static std::vector<char> ReadShader(std::string_view filepath);

	private:
		VkPipeline m_Pipeline;
		VkPipelineLayout m_Layout;
	};
}  // namespace At0::VulkanTesting
