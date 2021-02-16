#pragma once

#include <vulkan/vulkan_core.h>

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <optional>


namespace glslang
{
	class TProgram;
	class TType;
}  // namespace glslang


namespace At0::VulkanTesting
{
	class Shader
	{
	public:
		// Define added to the start of a shader
		struct Define
		{
			std::string Name;
			std::string Value;
		};

		// Class used to define sets of vertex inputs used in a shader
		class VertexInput
		{
		};

		class Uniform
		{
		};

		class UniformBlock
		{
		};

		class Attribute
		{
		};

		class Constant
		{
		};

	public:
		Shader();

		std::vector<VkDescriptorSetLayoutBinding> GetDescriptorSetLayoutBindings() const;
		std::vector<VkDescriptorPoolSize> GetDescriptorPoolSizes() const;

		static VkShaderStageFlagBits GetShaderStage(std::string_view filepath);


	private:
		std::vector<std::string> m_Stages;
		std::map<std::string, Uniform> m_Uniforms;
		std::map<std::string, UniformBlock> m_UniformBlocks;
		std::map<std::string, Attribute> m_Attributes;
		std::map<std::string, Constant> m_Constants;

		std::array<std::optional<uint32_t>, 3> m_LocalSizes;

		std::map<std::string, uint32_t> m_DescriptorLocations;
		std::map<std::string, uint32_t> m_DescriptorSizes;

		std::vector<VkDescriptorSetLayoutBinding> m_DescriptorSetLayouts;
		uint32_t m_LastDescriptorBinding = 0;
		std::vector<VkDescriptorPoolSize> m_DescriptorPools;
		std::map<uint32_t, VkDescriptorType> m_DescriptorTypes;
		std::vector<VkVertexInputAttributeDescription> m_AttributeDescriptions;

		mutable std::vector<std::string> m_NotFoundNames;
	};
}  // namespace At0::VulkanTesting
