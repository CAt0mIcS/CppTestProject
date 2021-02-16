#pragma once

#include <memory>

#include "Graphics/Vulkan/VertexBuffer.h"
#include "Graphics/Vulkan/IndexBuffer.h"
#include "Graphics/Vulkan/Pipeline.h"

#include "Graphics/Core/Codex.h"


#include "Graphics/Vulkan/Descriptor.h"
#include "Graphics/Vulkan/UniformBuffer.h"


namespace At0::VulkanTesting
{
	class CommandBuffer;

	class Drawable
	{
	public:
		void Draw(CommandBuffer& cmdBuff);

		const IndexBuffer& GetIndexBuffer() const { return *m_IndexBuffer; }
		const GraphicsPipeline& GetGraphicsPipeline() const { return *m_GraphicsPipeline; }

		virtual ~Drawable() = default;

		Scope<DescriptorSet> descriptorSet;
		Scope<UniformBuffer> uniformBuffer;

	protected:
		Drawable() = default;

		void EmplaceBindable(Ref<Bindable> bindable);

	private:
		std::vector<Ref<Bindable>> m_Bindables;
		IndexBuffer* m_IndexBuffer;
		GraphicsPipeline* m_GraphicsPipeline;
	};

}  // namespace At0::VulkanTesting
