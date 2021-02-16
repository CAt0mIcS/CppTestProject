#pragma once

#include <vulkan/vulkan_core.h>

#include <string>
#include <vector>


namespace At0::VulkanTesting
{
	class Shader
	{
	public:
		Shader() = default;

		std::vector<VkDescriptorSetLayoutBinding> GetDescriptorSetLayoutBindings() const;
		std::vector<VkDescriptorPoolSize> GetDescriptorPoolSizes() const;

		static VkShaderStageFlagBits GetShaderStage(std::string_view filepath);


	private:
	};
}  // namespace At0::VulkanTesting
