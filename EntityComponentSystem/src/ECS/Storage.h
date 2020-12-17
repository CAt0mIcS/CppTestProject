#pragma once

#include <stdint.h>
#include <vector>


namespace ECS
{
	template<typename Entity, typename Comp>
	class Storage
	{
	public:


	private:
		std::vector<Comp> m_Components;
	};

}

