#pragma once

#include "Base.h"
#include "Bindable.h"
#include "Buffer.h"

#include <vector>
#include <string_view>


namespace At0::VulkanTesting
{
	class IndexBuffer : public Buffer, public Bindable
	{
	public:
		using Type = uint16_t;

	public:
		IndexBuffer(const std::vector<Type>& indices, std::string_view tag);

		void Bind(const CommandBuffer& cmdBuff) override;
		uint32_t GetNumIndices() const;

		static std::string GetUID(const std::vector<Type>& indices, std::string_view tag);

	private:
		uint32_t m_NumIndices = 0;
		const std::string m_Tag;
	};
}  // namespace At0::VulkanTesting
