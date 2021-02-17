#include "pch.h"
#include "Graphics/Vulkan/UniformBuffer.h"
#include "UniformHandler.h"


namespace At0::VulkanTesting
{
	UniformHandler::UniformHandler()
		: m_Multipipeline(false), m_HandlerStatus(Buffer::Status::Normal)
	{
	}

	UniformHandler::UniformHandler(const Shader::UniformBlock& uniformBlock)
		: m_Multipipeline(false), m_UniformBlock(uniformBlock),
		  m_Size((uint32_t)uniformBlock.GetSize()),
		  m_UniformBuffer(MakeScope<UniformBuffer>((VkDeviceSize)m_Size)),
		  m_HandlerStatus(Buffer::Status::Normal)
	{
	}

	bool UniformHandler::Update(const std::optional<Shader::UniformBlock>& uniformBlock)
	{
		if (m_HandlerStatus == Buffer::Status::Reset || m_Multipipeline && !m_UniformBlock ||
			!m_Multipipeline && m_UniformBlock != uniformBlock)
		{
			if (m_Size == 0 && !m_UniformBlock || m_UniformBlock &&
													  m_UniformBlock != uniformBlock &&
													  (uint32_t)m_UniformBlock->GetSize() == m_Size)
			{
				m_Size = (uint32_t)uniformBlock->GetSize();
			}

			m_UniformBlock = uniformBlock;
			m_Bound = false;
			m_UniformBuffer = MakeScope<UniformBuffer>((VkDeviceSize)m_Size);
			m_HandlerStatus = Buffer::Status::Changed;
			return false;
		}

		if (m_HandlerStatus != Buffer::Status::Normal)
		{
			if (m_Bound)
			{
				m_UniformBuffer->UnmapMemory();
				m_Bound = false;
			}

			m_HandlerStatus = Buffer::Status::Normal;
		}

		return true;
	}
}  // namespace At0::VulkanTesting
