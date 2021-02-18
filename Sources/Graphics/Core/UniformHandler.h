#pragma once

#include "Base.h"

#include "Graphics/Vulkan/Shader.h"
#include "Graphics/Vulkan/UniformBuffer.h"

#include <optional>
#include <string.h>


namespace At0::VulkanTesting
{
	class UniformBuffer;

	class UniformHandler
	{
	public:
		UniformHandler();
		UniformHandler(const Shader::UniformBlock& uniformBlock);

		template<typename T>
		void Push(const T& object, size_t offset, size_t size)
		{
			if (!m_UniformBlock || !m_UniformBuffer)
				return;

			if (!m_Bound)
			{
				m_UniformBuffer->MapMemory(&this->m_Data);
				m_Bound = true;
			}

			// If the buffer is already changed we can skip a memory comparison and just copy.
			if (m_HandlerStatus == Buffer::Status::Changed ||
				memcmp(static_cast<char*>(this->m_Data), &object, size) != 0)
			{
				memcpy(static_cast<char*>(this->m_Data) + offset, &object, size);
				m_HandlerStatus = Buffer::Status::Changed;
			}
		}

		template<typename T>
		void Push(std::string_view uniformName, const T& object, std::size_t size = 0)
		{
			if (!m_UniformBlock || !m_UniformBuffer)
				return;

			auto uniform = m_UniformBlock->GetUniform(uniformName);
			if (!uniform)
				return;

			auto realSize = size;
			if (realSize == 0)
				realSize = std::min(sizeof(object), static_cast<std::size_t>(uniform->GetSize()));

			Push(object, static_cast<std::size_t>(uniform->GetOffset()), realSize);
		}

		bool Update(const std::optional<Shader::UniformBlock>& uniformBlock);

		const UniformBuffer& GetUniformBuffer() const { return *m_UniformBuffer; }

	private:
		bool m_Multipipeline;
		std::optional<Shader::UniformBlock> m_UniformBlock;
		uint32_t m_Size = 0;
		void* m_Data = nullptr;
		bool m_Bound = false;
		std::unique_ptr<UniformBuffer> m_UniformBuffer;
		Buffer::Status m_HandlerStatus;
	};
}  // namespace At0::VulkanTesting
