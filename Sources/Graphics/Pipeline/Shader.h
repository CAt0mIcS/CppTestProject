#pragma once

#include <vulkan/vulkan_core.h>
#include <string_view>
#include <vector>


namespace At0::VulkanTesting
{
	class Shader
	{
	public:
		Shader(std::string_view vertexShaderFilepath, std::string_view fragmentShaderFilepath);
		~Shader();

		const VkShaderModule& GetVertexShaderModule() const { return m_VertexShaderModule; }
		const VkShaderModule& GetFragmentShaderModule() const { return m_FragmentShaderModule; }

	private:
		static std::vector<char> ReadShader(std::string_view filepath);

	private:
		VkShaderModule m_VertexShaderModule;
		VkShaderModule m_FragmentShaderModule;
	};
}  // namespace At0::VulkanTesting
