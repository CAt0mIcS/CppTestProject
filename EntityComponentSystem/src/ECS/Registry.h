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

		};

	public:


	};

	using Registry = BasicRegistry<Entity>;
}
