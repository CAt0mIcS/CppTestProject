#pragma once

#include "Entity.h"


namespace At0::VulkanTesting
{
	class Scene
	{
	public:
		static Scene& Get();
		Entity CreateEntity();

	private:
		entt::registry m_EntityRegistry;
	};
}  // namespace At0::VulkanTesting
