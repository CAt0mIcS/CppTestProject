#pragma once

#include <vulkan/vulkan_core.h>

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <optional>
#include <filesystem>

#include "Base.h"


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
		public:
			VertexInput(std::vector<VkVertexInputBindingDescription> bindingDescriptions = {},
				std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {})
				: m_BindingDescriptions(std::move(bindingDescriptions)),
				  m_AttributeDescriptions(std::move(attributeDescriptions))
			{
			}

			const std::vector<VkVertexInputBindingDescription>& GetBindingDescriptions() const
			{
				return m_BindingDescriptions;
			}

			const std::vector<VkVertexInputAttributeDescription>& GetAttributeDescriptions() const
			{
				return m_AttributeDescriptions;
			}

			bool operator<(const VertexInput&& other) const
			{
				return m_BindingDescriptions.front().binding <
					   other.m_BindingDescriptions.front().binding;
			}

		private:
			uint32_t m_Binding = 0;
			std::vector<VkVertexInputBindingDescription> m_BindingDescriptions;
			std::vector<VkVertexInputAttributeDescription> m_AttributeDescriptions;
		};

		class Uniform
		{
			friend class Shader;

		public:
			Uniform(int32_t binding = -1, int32_t offset = -1, int32_t size = -1,
				int32_t glType = -1, bool readOnly = false, bool writeOnly = false,
				VkShaderStageFlags stageFlags = 0)
				: m_Binding(binding), m_Offset(offset), m_Size(size), m_GlType(glType),
				  m_ReadOnly(readOnly), m_WriteOnly(writeOnly), m_StageFlags(stageFlags)
			{
			}

			int32_t GetBinding() const { return m_Binding; }
			int32_t GetOffset() const { return m_Offset; }
			int32_t GetSize() const { return m_Size; }
			int32_t GetGlType() const { return m_GlType; }
			bool IsReadOnly() const { return m_ReadOnly; }
			bool IsWriteOnly() const { return m_WriteOnly; }
			VkShaderStageFlags GetStageFlags() const { return m_StageFlags; }

			bool operator==(const Uniform& other) const
			{
				return m_Binding == other.m_Binding && m_Offset == other.m_Offset &&
					   m_Size == other.m_Size && m_GlType == other.m_GlType &&
					   m_ReadOnly == other.m_ReadOnly && m_WriteOnly == other.m_WriteOnly &&
					   m_StageFlags == other.m_StageFlags;
			}

			bool operator!=(const Uniform& other) const { return !(*this == other); }

		private:
			int32_t m_Binding;
			int32_t m_Offset;
			int32_t m_Size;
			int32_t m_GlType;
			bool m_ReadOnly;
			bool m_WriteOnly;
			VkShaderStageFlags m_StageFlags;
		};

		class UniformBlock
		{
			friend class Shader;

		public:
			enum class Type
			{
				None,
				Uniform,
				Storage,
				Push
			};

			UniformBlock(int32_t binding = -1, int32_t size = -1, VkShaderStageFlags stageFlags = 0,
				Type type = Type::Uniform)
				: m_Binding(binding), m_Size(size), m_StageFlags(stageFlags), m_Type(type)
			{
			}

			int32_t GetBinding() const { return m_Binding; }
			int32_t GetSize() const { return m_Size; }
			VkShaderStageFlags GetStageFlags() const { return m_StageFlags; }
			Type GetType() const { return m_Type; }
			const std::map<std::string, Uniform>& GetUniforms() const { return m_Uniforms; }

			std::optional<Uniform> GetUniform(std::string_view name) const
			{
				auto it = m_Uniforms.find(name.data());

				if (it == m_Uniforms.end())
					return std::nullopt;

				return it->second;
			}

			bool operator==(const UniformBlock& other) const
			{
				return m_Binding == other.m_Binding && m_Size == other.m_Size &&
					   m_StageFlags == other.m_StageFlags && m_Type == other.m_Type &&
					   m_Uniforms == other.m_Uniforms;
			}

			bool operator!=(const UniformBlock& other) const { return !(*this == other); }

		private:
			int32_t m_Binding;
			int32_t m_Size;
			VkShaderStageFlags m_StageFlags;
			Type m_Type;
			std::map<std::string, Uniform> m_Uniforms;
		};

		class Attribute
		{
		public:
			Attribute(
				int32_t set = -1, int32_t location = -1, int32_t size = -1, int32_t glType = -1)
				: m_Set(set), m_Location(location), m_Size(size), m_GlType(glType)
			{
			}

			int32_t GetSet() const { return m_Set; }
			int32_t GetLocation() const { return m_Location; }
			int32_t GetSize() const { return m_Size; }
			int32_t GetGlType() const { return m_GlType; }

			bool operator==(const Attribute& other) const
			{
				return m_Set == other.m_Set && m_Location == other.m_Location &&
					   m_Size == other.m_Size && m_GlType == other.m_GlType;
			}

			bool operator!=(const Attribute& other) const { return !(*this == other); }

		private:
			int32_t m_Set;
			int32_t m_Location;
			int32_t m_Size;
			int32_t m_GlType;
		};

		class Constant
		{
		public:
			Constant(int32_t binding = -1, int32_t size = -1, VkShaderStageFlags stageFlags = 0,
				int32_t glType = -1)
				: m_Binding(binding), m_Size(size), m_StageFlags(stageFlags), m_GlType(glType)
			{
			}

			int32_t GetBinding() const { return m_Binding; }
			int32_t GetSize() const { return m_Size; }
			VkShaderStageFlags GetStageFlags() const { return m_StageFlags; }
			int32_t GetGlType() const { return m_GlType; }

			bool operator==(const Constant& other) const
			{
				return m_Binding == other.m_Binding && m_Size == other.m_Size &&
					   m_StageFlags == other.m_StageFlags && m_GlType == other.m_GlType;
			}

			bool operator!=(const Constant& other) const { return !(*this == other); }

		private:
			int32_t m_Binding;
			int32_t m_Size;
			VkShaderStageFlags m_StageFlags;
			int32_t m_GlType;
		};

	public:
		Shader();

		bool ReportedNotFound(std::string_view name, bool reportIfFound) const;
		static VkFormat GlTypeToVk(int32_t type);
		std::optional<uint32_t> GetDescriptorLocation(std::string_view name) const;
		std::optional<uint32_t> GetDescriptorSize(std::string_view name) const;
		std::optional<Uniform> GetUniform(std::string_view name) const;
		std::optional<UniformBlock> GetUniformBlock(std::string_view name) const;
		std::optional<Attribute> GetAttribute(std::string_view name) const;
		std::vector<VkPushConstantRange> GetPushConstantRanges() const;

		std::optional<VkDescriptorType> GetDescriptorType(uint32_t location) const;
		static VkShaderStageFlagBits GetShaderStage(const std::filesystem::path& filepath);
		VkShaderModule CreateShaderModule(const std::filesystem::path& moduleName,
			std::string_view moduleCode, std::string_view preamble, VkShaderStageFlags moduleFlag);
		void CreateReflection();

		std::string_view GetName() const { return m_Stages.back(); }
		uint32_t GetLastDescriptorBinding() const { return m_LastDescriptorBinding; }
		const auto& GetUniforms() const { return m_Uniforms; }
		const auto& GetUniformBlocks() const { return m_UniformBlocks; }
		const auto& GetAttributes() const { return m_Attributes; }
		const auto& GetConstants() const { return m_Constants; }
		const auto& GetLocalSizes() const { return m_LocalSizes; }
		const auto& GetDescriptorSetLayouts() const { return m_DescriptorSetLayouts; }
		const auto& GetDescriptorPools() const { return m_DescriptorPools; }
		const auto& GetAttributeDescriptions() const { return m_AttributeDescriptions; }

		static std::optional<std::string> ReadFile(const std::filesystem::path& filepath);

	private:
		static void IncrementDescriptorPool(
			std::map<VkDescriptorType, uint32_t>& descriptorPoolCounts, VkDescriptorType type);
		void LoadUniformBlock(
			const glslang::TProgram& program, VkShaderStageFlags stageFlag, int32_t i);
		void LoadUniform(
			const glslang::TProgram& prograam, VkShaderStageFlags stageFlag, int32_t i);
		void LoadAttribute(
			const glslang::TProgram& program, VkShaderStageFlags stageFlags, int32_t i);
		static int32_t ComputeSize(const glslang::TType* ttype);

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
