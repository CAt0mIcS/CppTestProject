#pragma once

#include <stdint.h>


namespace ECS
{
	using IndexType = uint32_t;
	inline constexpr uint32_t MAX_ENTITIES = 5000;
	inline constexpr uint32_t MAX_COMPONENTS = 50;

	namespace Internal
	{
		struct ComponentIndex
		{
			static IndexType Next()
			{
				static IndexType idx = 0;
				return idx++;
			}
		};
	}


	template<typename Component>
	struct ComponentIndex
	{
		static IndexType Value()
		{
			static IndexType next = Internal::ComponentIndex::Next();
			return next;
		}
	};

}

