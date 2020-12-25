#pragma once

#include "Internal.h"
#include "ComponentStorage.h"
#include "View.h"

#include <memory>

namespace ECS
{
	class Registry
	{
	public:
		template<typename Component, typename... Args>
		decltype(auto) Emplace(Entity e, Args&&... args)
		{
			return Assure<Component>().Emplace(e, std::forward<Args>(args)...);
		}

		template<typename... Component>
		decltype(auto) Get(Entity e)
		{
			if constexpr (sizeof...(Component) == 1)
			{
				return (Assure<Component>().Get(e), ...);
			}
			else
			{
				return std::forward_as_tuple(Get<Component>(e)...);
			}
		}

		Entity Create()
		{
			// TODO: Entity destruction mask

			return m_Entities.emplace_back((uint32_t)m_Entities.size());
		}

		template<typename... Component>
		View<Component...> View()
		{
			return { Assure<Component>()... };
		}

	private:
		struct PoolData
		{
			IndexType TypeID{};
			std::unique_ptr<EntityStorage> Pool{};
		};

		template<typename Component>
		struct PoolHandler : ComponentStorage<Component>
		{
			template<typename... Args>
			decltype(auto) Emplace(Entity e, Args&&... args)
			{
				return ComponentStorage<Component>::Emplace(e, std::forward<Args>(args)...);
			}

			Component& Get(Entity e)
			{
				return ComponentStorage<Component>::Get(e);
			}
		};

		template<typename Component>
		PoolHandler<Component>& Assure()
		{
			if constexpr (HasComponentIndex<Component>::value)
			{
				const auto index = ComponentIndex<Component>::Value();

				if (index >= m_Pools.size())
					m_Pools.resize(index + 1);

				if (auto&& pData = m_Pools[index]; !pData.Pool)
				{
					pData.TypeID = ComponentIndex<Component>::Value();
					pData.Pool.reset(new PoolHandler<Component>());
				}

				return (PoolHandler<Component>&)(*m_Pools[index].Pool);
			}
			else
			{
				EntityStorage* candidate{ nullptr };

				if (auto it = std::find_if(m_Pools.begin(), m_Pools.end(), [id = ComponentIndex<Component>::Value()](const auto& pData) { return id == pData.TypeID; }); it == m_Pools.cend())
				{
					candidate = m_Pools.emplace_back(PoolData{
						ComponentIndex<Component>::Value(),
						std::unique_ptr<EntityStorage>{new PoolHandler<Component>{}}
						}
					).Pool.get();
				}
				else
				{
					candidate = it->Pool.get();
				}
				return (PoolHandler<Component>&)(*candidate);
			}
		}

	private:
		std::vector<PoolData> m_Pools{};
		std::vector<Entity> m_Entities{};
		Entity m_Destroyed{EntityNull};
	};
}

