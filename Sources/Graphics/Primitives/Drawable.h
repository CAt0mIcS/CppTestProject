#pragma once

#include <memory>

#include "Graphics/Bindables/VertexBuffer.h"
#include "Graphics/Bindables/IndexBuffer.h"


namespace At0::VulkanTesting
{
	class Drawable
	{
	public:
		virtual VertexBuffer& GetVertexBuffer() = 0;
		virtual IndexBuffer& GetIndexBuffer() = 0;

		virtual VkCullModeFlags GetCullMode() const = 0;

		virtual ~Drawable() = default;

	protected:
		Drawable() = default;
	};


	template<typename T>
	class DrawableBase : public Drawable
	{
	public:
		virtual VertexBuffer& GetVertexBuffer() override { return *s_VertexBuffer; }
		virtual IndexBuffer& GetIndexBuffer() override { return *s_IndexBuffer; }

		virtual ~DrawableBase()
		{
			s_VertexBuffer.reset();
			s_IndexBuffer.reset();
		}

	protected:
		DrawableBase() = default;

	protected:
		inline static std::unique_ptr<VertexBuffer> s_VertexBuffer = nullptr;
		inline static std::unique_ptr<IndexBuffer> s_IndexBuffer = nullptr;
	};
}  // namespace At0::VulkanTesting
