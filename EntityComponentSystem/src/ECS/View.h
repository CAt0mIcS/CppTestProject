#pragma once

#include "Storage.h"
#include <tuple>


namespace ECS
{
	template<typename... Component>
	class View
	{
	private:
		// QUESTION: What?
		template<typename Comp>
		using PoolType = std::conditional_t<std::is_const_v<Comp>, const Storage<std::remove_const_t<Comp>>, Storage<Comp>>;

	public:
		View(Storage<Component>&... component)
			: m_Components{ &component... }
		{

		}



	private:
		std::tuple<PoolType<Component>*...> m_Components;
	};
}