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


	// If ComponentIndex<Component>::Value() already exists for the component then the type from this function (decltype)
	// will be converted to void and the std::true_type struct will be used because of template specialisation. If the function Value()
	// for the specific component has not been generated, then decltype will fail and the std::false_type struct will be used.

	template<typename, typename = void>
	struct HasComponentIndex : std::false_type {};

	template<typename Component>
	struct HasComponentIndex<Component, std::void_t<decltype(ComponentIndex<Component>::Value())>> : std::true_type {};
}

