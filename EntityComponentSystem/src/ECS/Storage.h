#pragma once

#include <stdint.h>
#include <vector>


namespace ECS
{
	template<typename Entity, typename Components>
	class Storage
	{
	public:


	private:
		std::vector<Components> m_Components;
	};

}

