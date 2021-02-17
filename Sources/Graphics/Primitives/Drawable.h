#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Graphics/Vulkan/VertexBuffer.h"
#include "Graphics/Vulkan/IndexBuffer.h"
#include "Graphics/Vulkan/GraphicsPipeline.h"

#include "Graphics/Core/Codex.h"


#include "Graphics/Core/UniformHandler.h"
#include "Graphics/Core/DescriptorsHandler.h"


namespace At0::VulkanTesting
{
	class CommandBuffer;

	class Drawable
	{
	public:
		void CmdDraw(const CommandBuffer& cmdBuff);
		void Update();

		const IndexBuffer& GetIndexBuffer() const { return *m_IndexBuffer; }
		const GraphicsPipeline& GetGraphicsPipeline() const { return *m_GraphicsPipeline; }

		void Translate(glm::vec3 translation) { m_Translation += translation; }
		void Scale(glm::vec3 scale) { m_Scale += scale; }
		void Rotate(glm::vec3 rotation) { m_Rotation += rotation; }

		virtual ~Drawable() = default;

	protected:
		Drawable() = default;

		void EmplaceBindable(Ref<Bindable> bindable);

	private:
		std::vector<Ref<Bindable>> m_Bindables;
		IndexBuffer* m_IndexBuffer;
		GraphicsPipeline* m_GraphicsPipeline;

		glm::vec3 m_Translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale{ 1.0f, 1.0f, 1.0f };
		glm::vec3 m_Rotation{ 0.0f, 0.0f, 0.0f };

		DescriptorsHandler m_DescriptorsHandler;
		UniformHandler m_UniformHandler;
	};

}  // namespace At0::VulkanTesting
