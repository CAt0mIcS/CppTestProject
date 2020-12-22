#pragma once

#include "Storage.h"
#include <tuple>

template<bool Test, typename T1, typename T2>
struct conditional
{
	using type = T1;
};


template<typename T1, typename T2>
struct conditional<false, T1, T2>
{
	using type = T2;
};

template<bool Test, typename T1, typename T2>
using conditional_t = typename conditional<Test, T1, T2>::type;

namespace ECS
{
	template<typename... Component>
	class View
	{
	private:
		// If the Component (Comp) is const then PoolType will be "const Storage<std::remove_const_t<Comp>>" else just "Storage<Comp>"
		template<typename Comp>
		using PoolType = conditional_t<std::is_const_v<Comp>, const Storage<std::remove_const_t<Comp>>, Storage<Comp>>;

	public:
		View(Storage<Component>&... component)
			: m_Components{ &component... }
		{

		}



	private:
		std::tuple<PoolType<Component>*...> m_Components;
	};
}