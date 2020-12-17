#pragma once

#include "Internal.h"
#include "Entity.h"
#include "Storage.h"
#include <vector>
#include <unordered_map>
#include <stdint.h>


namespace ECS
{
	class Registry
	{
	public:
		template<typename Comp, typename... Args>
		decltype(auto) Emplace(Entity entity, Args&&... args)
		{
			return Assure<Comp>().Emplace(entity, std::forward<Args>(args)...);
		}

	private:
		template<typename Component>
		struct PoolHandler : Storage<Component>
		{
			template<typename... Args>
			decltype(auto) Emplace(Entity entity, Args&&... args)
			{
				Storage<Component>::Emplace(std::forward<Args>(args)...);
			}
		};


		template<typename Comp>
		PoolHandler<Comp>& Assure()
		{

		}

		struct PoolData
		{
			uint32_t TypeID{};
		};

	private:
		uint32_t m_EntityCounter = 0;
		std::vector<PoolData> m_Pools;
	};
}
