#pragma once

#include "REntityStorage.h"


namespace At0::Ray::ECS
{
	template<typename Component>
	class ComponentStorage : public EntityStorage
	{
	public:
		template<typename... Args>
		decltype(auto) Emplace(Entity e, Args&&... args)
		{
			if (m_FreeIndices.size() > 0)
			{
				if constexpr (std::is_aggregate_v<Component>)
					m_Components[m_FreeIndices.back()] = Component{ std::forward<Args>(args)... };
				else
					m_Components[m_FreeIndices.back()] = Component(std::forward<Args>(args)...);

				EntityStorage::SetComponentIndex(e, (uint32_t)m_FreeIndices.back());
				m_FreeIndices.erase(m_FreeIndices.end() - 1);
			}
			else
			{
				if constexpr (std::is_aggregate_v<Component>)
					m_Components.push_back(Component{ std::forward<Args>(args)... });
				else
					m_Components.emplace_back(std::forward<Args>(args)...);

				EntityStorage::Emplace(e, (uint32_t)(m_Components.size() - 1));
			}
		}

		void Remove(Entity e)
		{
			m_FreeIndices.emplace_back(EntityStorage::IndexInComponentVector(e));
		}

		Component& Get(Entity e)
		{
			return const_cast<Component&>(std::as_const(*this).Get(e));
		}

		const Component& Get(Entity e) const
		{
			return m_Components[EntityStorage::IndexInComponentVector(e)];
		}

	private:
		virtual void RemoveEntity(Entity e) override
		{
			// RAY_TODO: Also erase entity in EntityStorage
			m_FreeIndices.emplace_back(EntityStorage::IndexInComponentVector(e));
			EntityStorage::Delete(e);
		}

	private:
		std::vector<Component> m_Components;
		std::vector<uint32_t> m_FreeIndices;
	};
}



