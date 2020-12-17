#pragma once

#include "Entity.h"


namespace ECS
{
	template<typename Component>
	class Storage
	{
	public:
		template<typename... Args>
		decltype(auto) Emplace(Args&&... args)
		{
			if constexpr (std::is_aggregate_v<Component>)
			{
				m_Components.push_back(Component{std::forward<Args>(args)...});
			}
			else
			{
				m_Components.emplace_back(std::forward<Args>(args)...);
			}
		}

	private:
		std::vector<Component> m_Components;
	};
}

