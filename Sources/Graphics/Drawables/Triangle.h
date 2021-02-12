#pragma once

#include "Graphics/Bindables/VertexBuffer.h"


namespace At0::VulkanTesting
{
	class Triangle
	{
	public:
		Triangle();
		~Triangle();

		const VertexBuffer& GetVertexBuffer() const { return m_VertexBuffer; }

	private:
		VertexBuffer m_VertexBuffer;
	};
}  // namespace At0::VulkanTesting
