#pragma once

#include "Internal.h"
#include <unordered_map>
#include <assert.h>


namespace ECS
{
	class Entity
	{
		using IDType = uint32_t;
	public:
		Entity()
			: m_ID(m_IDCounter++)
		{
		}

		Entity(IDType id)
			: m_ID(id)
		{
		}

		operator uint32_t()
		{
			return m_ID;
		}

		template<typename Component, typename... Args>
		decltype(auto) Add(Args&&... args)
		{
			return Storage<Component>::Emplace(m_ID, std::forward<Args>(args)...);
		}

		template<typename Component>
		Component& Get()
		{
			return const_cast<Component&>(std::as_const(*this).Get<Component>());
		}

		template<typename Component>
		const Component& Get() const
		{
			assert(Has<Component>() && "Entity does not have Component");
			return Storage<Component>::Get(m_ID);
		}

		template<typename Component>
		bool Has() const
		{
			return Storage<Component>::Has(m_ID);
		}

		template<typename Component>
		void Remove()
		{
			Storage<Component>::Remove(m_ID);
		}

	private:
		template<typename Component>
		struct Storage
		{
			template<typename... Args>
			static decltype(auto) Emplace(IDType entityID, Args&&... args)
			{
				m_EntitiesWithComponent.emplace_back(entityID);
				if constexpr (std::is_aggregate_v<Component>)
					m_Components[entityID] = Component{ std::forward<Args>(args)... };
				else
					m_Components[entityID] = Component(std::forward<Args>(args)...);

				return m_Components[entityID];
			}

			static Component& Get(IDType entityID)
			{
				return m_Components[entityID];
			}

			static bool Has(IDType entityID)
			{
				return std::find(m_EntitiesWithComponent.begin(), m_EntitiesWithComponent.end(), entityID) != m_EntitiesWithComponent.end();
			}

			static void Remove(IDType entityID)
			{
				if (auto id = std::find(m_EntitiesWithComponent.begin(), m_EntitiesWithComponent.end(), entityID); id != m_EntitiesWithComponent.end())
					m_EntitiesWithComponent.erase(id);

				m_Components.erase(entityID);
			}

		private:
			static std::vector<IDType> m_EntitiesWithComponent;
			static std::unordered_map<IDType, Component> m_Components;
		};

	private:
		uint32_t m_ID;
		inline static uint32_t m_IDCounter = 0;
	};

	template<typename Component>
	std::unordered_map<Entity::IDType, Component> Entity::Storage<Component>::m_Components;
	
	template<typename Component>
	std::vector<Entity::IDType> Entity::Storage<Component>::m_EntitiesWithComponent;
}
