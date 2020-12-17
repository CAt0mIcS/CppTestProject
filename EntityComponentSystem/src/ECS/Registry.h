#pragma once

#include "Internal.h"
#include "Entity.h"
#include <vector>
#include <unordered_map>
#include <stdint.h>


namespace ECS
{
	class Registry
	{
		template<typename Component>
		struct PoolHandler
		{
			template<typename... Args>
			static decltype(auto) Emplace(Entity entity, Args&&... args)
			{
				//// Entity entry doesn't exist in the component vector
				//if (m_EntityComponents.find(entity) == m_EntityComponents.end())
				//{
				//}
				//// Entity entry does exist in the component vector
				//else
				//{

				//}
				return m_EntityComponents[entity].emplace_back(std::forward<Args>(args)...);
			}

			//m_Components[EntityID][ComponentID]
			static std::unordered_map<Entity::IDType, std::vector<Component>> m_EntityComponents;
		};

	public:
		template<typename Comp, typename... Args>
		decltype(auto) Emplace(Entity entity, Args&&... args)
		{
			return PoolHandler<Comp>::Emplace(entity, std::forward<Args>(args)...);
		}

	private:
		uint32_t m_EntityCounter = 0;
	};

	template<typename Component>
	inline std::unordered_map<Entity::IDType, std::vector<Component>> Registry::PoolHandler<Component>::m_EntityComponents;
}
