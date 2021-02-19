#pragma once

#include "Base.h"

#include <vector>
#include <string>

#include "Graphics/Vulkan/UniformBuffer.h"
#include "Graphics/Vulkan/Descriptor.h"
#include "Graphics/Vulkan/Shader.h"


namespace At0::VulkanTesting
{
	class Pipeline;
	class CommandBuffer;

	struct UniformData
	{
		UniformBuffer* uniformBuffer;
		std::vector<std::string> names;
	};

	class UniformView
	{
	public:
		UniformView(UniformBuffer* uBuff, int32_t offset) : m_UniformBuffer(uBuff), m_Offset(offset)
		{
		}

		template<typename T>
		UniformView& operator=(const T& data)
		{
			if (m_UniformBuffer)
				m_UniformBuffer->Update(&data, m_Offset);
			return *this;
		}

	private:
		UniformBuffer* m_UniformBuffer;
		int32_t m_Offset;
	};

	class UniformBlockView
	{
	public:
		UniformBlockView(UniformData* data, std::optional<Shader::UniformBlock> uniformBlock)
			: m_UniformData(data), m_UniformBlock(uniformBlock)
		{
		}

		UniformView operator[](std::string_view uniformName)
		{
			if (!m_UniformBlock || !m_UniformData)
				return { nullptr, 0 };

			if (auto it = std::find(
					m_UniformData->names.begin(), m_UniformData->names.end(), uniformName.data());
				it != m_UniformData->names.end())
			{
				return { m_UniformData->uniformBuffer,
					m_UniformBlock->GetUniform(uniformName)->GetOffset() };
			}

			return { nullptr, 0 };
		}

	private:
		UniformData* m_UniformData;
		std::optional<Shader::UniformBlock> m_UniformBlock;
	};

	class UniformHandler
	{
	public:
		UniformHandler(const Pipeline& pipeline);
		~UniformHandler();

		void Bind(const CommandBuffer& cmdBuff, const Pipeline& pipeline);

		UniformBlockView operator[](std::string_view uniformBlockName);

	private:
		std::unordered_map<std::string, UniformData> m_Uniforms;
		Scope<DescriptorSet> m_DescriptorSet;
		const Shader& m_Shader;
	};
}  // namespace At0::VulkanTesting
