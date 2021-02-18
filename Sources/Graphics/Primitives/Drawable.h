#pragma once


#include <memory>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "Base.h"
#include "Graphics/Core/Entity.h"
#include "Graphics/Core/Scene.h"

#include "Graphics/Core/UniformHandler.h"


namespace At0::VulkanTesting
{
	class CommandBuffer;
	class GraphicsPipeline;
	class IndexBuffer;
	class Bindable;


	class Drawable
	{
	public:
		void CmdBind(const CommandBuffer& cmdBuff);
		virtual void CmdDraw(const CommandBuffer& cmdBuff);
		void Update();

		const IndexBuffer& GetIndexBuffer() const { return *m_IndexBuffer; }
		const GraphicsPipeline& GetGraphicsPipeline() const { return *m_GraphicsPipeline; }

		Entity& GetEntity() { return m_Entity; }
		const Entity& GetEntity() const { return m_Entity; }

		virtual ~Drawable();

		Scope<UniformHandler> uniformHandler;

	protected:
		Drawable() = default;

		void EmplaceBindable(Ref<Bindable> bindable);

	private:
		std::vector<Ref<Bindable>> m_Bindables;
		IndexBuffer* m_IndexBuffer;
		GraphicsPipeline* m_GraphicsPipeline;

		Entity m_Entity = Scene::Get().CreateEntity();
	};

}  // namespace At0::VulkanTesting
