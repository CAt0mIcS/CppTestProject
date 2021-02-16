#include "Shader.h"
#include "Graphics/Graphics.h"
#include "Utils/RString.h"

#include <glslang/SPIRV/GlslangToSpv.h>


namespace At0::VulkanTesting
{
	Shader::Shader() {}
	std::vector<VkDescriptorSetLayoutBinding> Shader::GetDescriptorSetLayoutBindings() const
	{
		VkDescriptorSetLayoutBinding descBinding{};
		descBinding.binding = 0;
		descBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descBinding.descriptorCount = 1;
		descBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		descBinding.pImmutableSamplers = nullptr;

		return { descBinding };
	}

	std::vector<VkDescriptorPoolSize> Shader::GetDescriptorPoolSizes() const
	{
		VkDescriptorPoolSize poolSize{};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = Graphics::Get().GetSwapchain().GetNumberOfImages();

		return { poolSize };
	}

	VkShaderStageFlagBits Shader::GetShaderStage(std::string_view filepath)
	{
		if (String::EndsWith(filepath, ".comp.spv"))
			return VK_SHADER_STAGE_COMPUTE_BIT;
		if (String::EndsWith(filepath, ".vert.spv"))
			return VK_SHADER_STAGE_VERTEX_BIT;
		if (String::EndsWith(filepath, ".tesc.spv"))
			return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		if (String::EndsWith(filepath, ".tese.spv"))
			return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		if (String::EndsWith(filepath, ".geom.spv"))
			return VK_SHADER_STAGE_GEOMETRY_BIT;
		if (String::EndsWith(filepath, ".frag.spv"))
			return VK_SHADER_STAGE_FRAGMENT_BIT;
		return VK_SHADER_STAGE_ALL;
	}
}  // namespace At0::VulkanTesting
