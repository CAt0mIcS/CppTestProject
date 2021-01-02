#pragma once

#include "RInternal.h"
#include "RComponentStorage.h"

#include <memory>


namespace At0::Ray::ECS
{
	class Registry
	{
	public:
		Registry()
			: m_Entities{}, m_FreeEntities{}, m_Pools{}
		{

		}

		template<typename Component, typename... Args>
		decltype(auto) Emplace(Entity e, Args&&... args)
		{
			assert(IsValid(e), "[Registry::Emplace] Entity (ID={0}) is invalid.", e);
			return GetStorage<Component>().Emplace(e, std::forward<Args>(args)...);
		}

		Entity Create()
		{
			if (m_FreeEntities.size() > 0)
			{
				uint32_t index = m_FreeEntities.back();
				m_Entities[index] = index;
				m_FreeEntities.erase(m_FreeEntities.end() - 1);
				return index;
			}
			else
			{
				return m_Entities.emplace_back((Entity)m_Entities.size());
			}
		}

		template<typename... Component>
		void Remove(Entity e)
		{
			assert(IsValid(e), "[Registry::Remove] Entity (ID={0}) is invalid.", e);
			(GetStorage<Component>().Remove(e), ...);
		}

		bool IsValid(Entity e) const
		{
			return std::find(m_FreeEntities.begin(), m_FreeEntities.end(), e) == m_FreeEntities.end();
		}

		void Destroy(Entity e)
		{
			assert(IsValid(e), "[Registry::Destroy] Entity (ID={0}) is invalid.", e);
			for (std::unique_ptr<EntityStorage>& pool : m_Pools)
			{
				if (pool && pool->Contains(e))
					pool->RemoveEntity(e);
			}

			m_FreeEntities.emplace_back(e);
		}

		template<typename... Component>
		void Erase(Entity e)
		{
			assert(IsValid(e), "[Registry::Erase] Entity (ID={0}) is invalid.", e);
			for (std::unique_ptr<EntityStorage>& pool : m_Pools)
			{
				if (pool->Contains(e))
					(((ComponentStorage<Component>&)(*pool)).Remove(e), ...);
			}
		}

		template<typename... Component>
		decltype(auto) Get(Entity e)
		{
			assert(IsValid(e), "[Registry::Get] Entity (ID={0}) is invalid.", e);
			if constexpr (sizeof...(Component) == 1)
			{
				return (GetStorage<Component>().Get(e), ...);
			}
			else
			{
				return std::forward_as_tuple(Get<Component>(e)...);
			}
		}

		template<typename... Component>
		bool Has(Entity e) const
		{
			assert(IsValid(e), "[Registry::Has] Entity (ID={0}) is invalid.", e);
			return (GetStorage<Component>().Contains(e) && ...);
		}

		template<typename... Component>
		bool Any(Entity e) const
		{
			assert(IsValid(e), "[Registry::Any] Entity (ID={0}) is invalid.", e);
			return (GetStorage<Component>().Contains(e) || ...);
		}

	private:
		template<typename Component>
		const ComponentStorage<Component>& GetStorage() const
		{
			auto index = ComponentIndex<Component>::Value();

			if (index >= m_Pools.size())
				m_Pools.resize(((size_t)index) + 1);

			if (std::unique_ptr<EntityStorage>& eStorage = m_Pools[index]; !eStorage)
			{
				eStorage = std::make_unique<ComponentStorage<Component>>();
			}

			return (ComponentStorage<Component>&)(*m_Pools[index]);
		}

		template<typename Component>
		ComponentStorage<Component>& GetStorage()
		{
			return const_cast<ComponentStorage<Component>&>(std::as_const(*this).GetStorage<Component>());
		}

	private:
		mutable std::vector<std::unique_ptr<EntityStorage>> m_Pools;
		std::vector<uint32_t> m_FreeEntities;
		std::vector<Entity> m_Entities;
	};
}
