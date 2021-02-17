#pragma once

#include <vector>
#include <array>
#include <string>

#include "Buffer.h"
#include "Bindable.h"
#include "Vertex.h"


namespace At0::VulkanTesting
{
	class VertexBuffer : public Buffer, public Bindable
	{
	public:
		VertexBuffer(const VertexInput& vertices, std::string_view tag);

		void Bind(const CommandBuffer& cmdBuff) override;

		static std::string GetUID(const VertexInput& vertices, std::string_view tag);
	};
}  // namespace At0::VulkanTesting
