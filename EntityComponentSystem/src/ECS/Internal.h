#pragma once

#include <stdint.h>
#include <type_traits>


namespace ECS
{
	using IndexType = uint32_t;
	using Entity = uint32_t;
	inline constexpr uint32_t EntityNull = -1;

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

	template<typename Component, typename = void>
	struct ComponentInfo
	{
		static uint32_t ID()
		{
			return ComponentIndex<Component>::Value();
		}
	};

	// QUESTION: What?
	template<typename, typename = void>
	struct HasComponentIndex : std::false_type {};

	template<typename Component>
	struct HasComponentIndex<Component, std::void_t<decltype(ComponentIndex<Component>::Value())>> : std::true_type {};
}

