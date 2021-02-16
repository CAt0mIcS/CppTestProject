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
		void CmdDraw(CommandBuffer& cmdBuff);
		void Update();

		const IndexBuffer& GetIndexBuffer() const { return *m_IndexBuffer; }
		const GraphicsPipeline& GetGraphicsPipeline() const { return *m_GraphicsPipeline; }
		const DescriptorSet& GetDescriptorSet() const { return *m_DescriptorSet; }
		const UniformBuffer& GetUniformBuffer() const { return *m_UniformBuffer; }

		void Translate(glm::vec3 translation) { m_Translation += translation; }
		void Scale(glm::vec3 scale) { m_Scale += scale; }
		void Rotate(glm::vec3 rotation) { m_Rotation += rotation; }

		virtual ~Drawable() = default;

	protected:
		Drawable() = default;

		void EmplaceBindable(Ref<Bindable> bindable);
		void InternalInit();

	private:
		std::vector<Ref<Bindable>> m_Bindables;
		IndexBuffer* m_IndexBuffer;
		GraphicsPipeline* m_GraphicsPipeline;

		Scope<DescriptorSet> m_DescriptorSet;
		Scope<UniformBuffer> m_UniformBuffer;

		glm::vec3 m_Translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale{ 1.0f, 1.0f, 1.0f };
		glm::vec3 m_Rotation{ 0.0f, 0.0f, 0.0f };
	};

}  // namespace At0::VulkanTesting
