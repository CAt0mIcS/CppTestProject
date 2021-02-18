#pragma once

#include "Base.h"

#include <entt/entt.hpp>

#include "Components.h"
#include "Utils/RAssert.h"


namespace At0::VulkanTesting
{
	class Entity
	{
	public:
		Entity(entt::registry& registry) : m_Registry(registry), m_EntityHandle(registry.create())
		{
		}

		template<typename Component, typename... Args>
		void Emplace(Args&&... args)
		{
			RAY_MEXPECTS(!Has<Component>(), "Entity already has component.");
			m_Registry.emplace<Component>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename... Component>
		bool Has() const
		{
			return m_Registry.all_of<Component...>(m_EntityHandle);
		}

		template<typename... Component>
		decltype(auto) Get()
		{
			RAY_MEXPECTS(Has<Component...>(), "Entity does not have component.");
			return m_Registry.get<Component...>(m_EntityHandle);
		}

		template<typename... Component>
		decltype(auto) Get() const
		{
			RAY_MEXPECTS(Has<Component...>(), "Entity does not have component.");
			return m_Registry.get<Component...>(m_EntityHandle);
		}

		template<typename... Component>
		void Erase()
		{
			RAY_MEXPECTS(Has<Component...>(), "Entity does not have component.");
			m_Registry.remove<Component...>(m_EntityHandle);
		}

	private:
		entt::entity m_EntityHandle;
		entt::registry& m_Registry;
	};
}  // namespace At0::VulkanTesting
