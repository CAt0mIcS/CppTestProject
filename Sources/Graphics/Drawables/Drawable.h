#pragma once

#include <memory>

#include "Graphics/Bindables/VertexBuffer.h"


namespace At0::VulkanTesting
{
	class Drawable
	{
	public:
		virtual VertexBuffer& GetVertexBuffer() = 0;

		virtual ~Drawable() = default;

	protected:
		Drawable() = default;
	};


	template<typename T>
	class DrawableBase : public Drawable
	{
	public:
		virtual VertexBuffer& GetVertexBuffer() override { return *s_VertexBuffer; }

		virtual ~DrawableBase() { s_VertexBuffer.reset(); }

	protected:
		DrawableBase() = default;

	protected:
		inline static std::unique_ptr<VertexBuffer> s_VertexBuffer = nullptr;
	};
}  // namespace At0::VulkanTesting
