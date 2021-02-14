#pragma once

#include "Buffer.h"

#include <vector>

namespace At0::VulkanTesting
{
	class IndexBuffer : public Buffer
	{
	public:
		IndexBuffer(const std::vector<uint16_t>& indices);
		~IndexBuffer() = default;

		void Bind(CommandBuffer& cmdBuff) override;

		uint32_t GetNumIndices() const;

	private:
		uint32_t m_NumIndices = 0;
	};
}  // namespace At0::VulkanTesting
