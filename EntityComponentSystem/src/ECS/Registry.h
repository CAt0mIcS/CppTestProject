#pragma once

#include "Internal.h"
#include "Entity.h"
#include "Storage.h"

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

	private:
		struct PoolData
		{
			Entity e;
			
		};

		template<typename Component>
		struct PoolHandler : public Storage<Component>
		{
			template<typename... Args>
			decltype(auto) Emplace(Entity e, Args&&... args)
			{
				Storage<Component>::Emplace(e, std::forward<Args>(args)...);

				if constexpr (!std::is_empty_v<Component>)
				{
					return this->Get(e);
				}
			}
		};

		template<typename Component>
		PoolHandler<Component>& Assure()
		{

		}


	};
}

