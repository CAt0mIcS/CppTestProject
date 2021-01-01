#pragma once

#include <stdint.h>
#include <type_traits>


namespace At0::Ray::ECS::Internal
{
	struct ComponentIndex
	{
		static const uint32_t Value()
		{
			static uint32_t next = 0;
			return next++;
		}
	};
}

namespace At0::Ray::ECS
{
	using Entity = uint32_t;

	template<typename Component>
	struct ComponentIndex
	{
		static const uint32_t Value()
		{
			static const uint32_t value = Internal::ComponentIndex::Value();
			return value;
		}
	};


	template<typename, typename = void>
	struct HasComponentIndex : std::false_type {};

	template<typename Component>
	struct HasComponentIndex<Component, std::void_t<decltype(ComponentIndex<Component>::Value())>> : std::true_type {};
}

