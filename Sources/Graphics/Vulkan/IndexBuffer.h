#pragma once

#include "Bindable.h"
#include "Buffer.h"

#include <vector>
#include <string_view>


namespace At0::VulkanTesting
{
	class IndexBuffer : public Buffer, public Bindable
	{
	public:
		IndexBuffer(std::string_view tag, const std::vector<uint16_t>& indices);

		void Bind(const CommandBuffer& cmdBuff) override;
		uint32_t GetNumIndices() const;

		static std::string GetUID(std::string_view tag, const std::vector<uint16_t>& indices);

	private:
		uint32_t m_NumIndices = 0;
		const std::string m_Tag;
	};
}  // namespace At0::VulkanTesting
