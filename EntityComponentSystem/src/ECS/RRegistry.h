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
			: m_NextEntity(0), m_Pools{}
		{

		}

		template<typename Component, typename... Args>
		decltype(auto) Emplace(Entity e, Args&&... args)
		{
			return GetStorage<Component>().Emplace(e);
		}

		Entity Create()
		{
			return m_NextEntity++;
		}

		template<typename Component>
		void Remove(Entity e)
		{

		}

		void Destroy(Entity e)
		{
			for (std::unique_ptr<EntityStorage>& pool : m_Pools)
			{
				pool->RemoveEntity(e);
			}
		}

		template<typename... Component>
		decltype(auto) Get(Entity e)
		{
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
			return (GetStorage<Component>().Contains(e) && ...);
		}

		template<typename... Component>
		bool Any(Entity e) const
		{
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
		uint32_t m_NextEntity;
	};
}
