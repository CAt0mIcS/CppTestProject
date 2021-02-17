#include "pch.h"
#include "Scene.h"

#include "Components.h"


namespace At0::VulkanTesting
{
	Scene& Scene::Get()
	{
		static Scene scene;
		return scene;
	}

	Entity Scene::CreateEntity()
	{
		Entity e(m_EntityRegistry);
		e.Emplace<TransformComponent>();

		return e;
	}
}  // namespace At0::VulkanTesting
