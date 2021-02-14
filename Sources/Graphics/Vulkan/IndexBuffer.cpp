#include "IndexBuffer.h"
#include "Utils/RAssert.h"

#include "Graphics/Commands/CommandBuffer.h"

namespace At0::VulkanTesting
{
	IndexBuffer::IndexBuffer(const std::vector<uint16_t>& indices)
		: Buffer(sizeof(indices[0]) * indices.size(), indices.data(),
			  VK_BUFFER_USAGE_INDEX_BUFFER_BIT),
		  m_NumIndices((uint32_t)indices.size())
	{
	}

	void IndexBuffer::Bind(CommandBuffer& cmdBuff)
	{
		vkCmdBindIndexBuffer(cmdBuff, m_Buffer, 0, VK_INDEX_TYPE_UINT16);
	}

	uint32_t IndexBuffer::GetNumIndices() const
	{
		RAY_MEXPECTS(m_NumIndices != 0, "Didn't set number of indices in the index buffer");
		return m_NumIndices;
	}
}  // namespace At0::VulkanTesting
