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
		struct PoolHandler final : public Storage<Entity, Component>
		{
			template<typename... Args>
			decltype(auto) Emplace(Entity entity, Args&&... args)
			{
				
			}
		};

	public:
		Entity Create()
		{
			return Entity{};
		}

		template<typename Comp, typename... Args>
		decltype(auto) Emplace(Entity entity, Args&&... args)
		{
			//return Assure<Comp>::Emplace(entity, std::forward<Args>(args)...);
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

	};

	using Registry = BasicRegistry<Entity>;
}
