#pragma once

#include "Internal.h"
#include <vector>
#include <stdint.h>


namespace ECS
{
	template<typename Entity>
	class BasicRegistry
	{
		template<typename Component>
		struct PoolHandler final/* : public Storage<Entity, Component>*/
		{
			template<typename... Args>
			static decltype(auto) Emplace(Entity entity, Args&&... args)
			{
				// Entity entry doesn't exist in the component vector
				if (entity >= m_Components.size())
				{
					std::vector<Component> comps;
					comps.emplace_back(std::forward<Args>(args)...);
					m_Components.emplace_back(std::move(comps));
				}
				// Does exist
				else
				{
					m_Components[entity].emplace_back(std::forward<Args>(args)...);
				}

				return m_Components.back().back();
			}

			//m_Components[EntityID][ComponentID]
			static std::vector<std::vector<Component>> m_Components;
		};

	public:
		Entity Create()
		{
			return m_EntityCounter++;
		}

		template<typename Comp, typename... Args>
		decltype(auto) Emplace(Entity entity, Args&&... args)
		{
			return PoolHandler<Comp>::Emplace(entity, std::forward<Args>(args)...);
		}


	private:
		template<typename Comp>
		PoolHandler<Comp>& Assure()
		{
			if constexpr (HasTypeIndex<Comp>::Value)
			{
				const auto index = TypeIndex<Comp>::Value();
			}
			else
			{

			}
		}


	private:
		uint32_t m_EntityCounter = 0;
	};

	template<typename Entity>
	template<typename Component>
	inline std::vector<std::vector<Component>> BasicRegistry<Entity>::PoolHandler<Component>::m_Components;

	using Registry = BasicRegistry<Entity>;
}
