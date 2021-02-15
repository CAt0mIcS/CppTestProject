#pragma once

#include <memory>

#include "Graphics/Vulkan/VertexBuffer.h"
#include "Graphics/Vulkan/IndexBuffer.h"

#include "Graphics/Core/Codex.h"


namespace At0::VulkanTesting
{
	class CommandBuffer;

	class Drawable
	{
	public:
		void Draw(CommandBuffer& cmdBuff);
		uint32_t GetNumberOfIndices() const { return m_NumIndices; }

		virtual ~Drawable() = default;

	protected:
		Drawable() = default;

		void EmplaceBindable(std::shared_ptr<Bindable> bindable);

	private:
		std::vector<std::shared_ptr<Bindable>> m_Bindables;
		uint32_t m_NumIndices;
	};

}  // namespace At0::VulkanTesting
