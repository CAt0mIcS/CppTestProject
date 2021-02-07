#include "Shader.h"
#include "Graphics/Graphics.h"

#include <fstream>
#include <filesystem>


namespace At0::VulkanTesting
{
	Shader::Shader(std::string_view vertexShaderFilepath, std::string_view fragmentShaderFilepath)
	{
		// Vertex shader
		{
			std::vector<char> code = ReadShader(vertexShaderFilepath);

			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = code.size();
			createInfo.pCode = reinterpret_cast<uint32_t*>(code.data());

			RAY_VK_THROW_FAILED(vkCreateShaderModule(Graphics::Get().GetLogicalDevice(),
									&createInfo, nullptr, &m_VertexShaderModule),
				"Failed to create vertex shader module.");
		}
		// Fragment shader
		{
			std::vector<char> code = ReadShader(fragmentShaderFilepath);

			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = code.size();
			createInfo.pCode = reinterpret_cast<uint32_t*>(code.data());

			RAY_VK_THROW_FAILED(vkCreateShaderModule(Graphics::Get().GetLogicalDevice(),
									&createInfo, nullptr, &m_FragmentShaderModule),
				"Failed to create fragment shader module.");
		}
	}

	Shader::~Shader()
	{
		vkDestroyShaderModule(Graphics::Get().GetLogicalDevice(), m_VertexShaderModule, nullptr);
		vkDestroyShaderModule(Graphics::Get().GetLogicalDevice(), m_FragmentShaderModule, nullptr);
	}

	std::vector<char> Shader::ReadShader(std::string_view filepath)
	{
		std::ifstream reader(filepath.data(), std::ios::ate | std::ios::binary);

		size_t filesize = (size_t)reader.tellg();
		RAY_MEXPECTS(filesize != 0, "Shader file not found");
		std::vector<char> code(filesize);

		reader.seekg(0);
		reader.read(code.data(), filesize);
		reader.close();

		return code;
	}
}  // namespace At0::VulkanTesting
