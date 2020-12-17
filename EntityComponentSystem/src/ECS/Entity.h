#pragma once

#include <stdint.h>
#include <vector>


namespace ECS
{
	class Entity
	{
	public:
		using IDType = uint32_t;

	public:
		Entity()
			: m_ID(s_EntityCounter++) {}

		operator IDType() const { return m_ID; }

	private:
		IDType m_ID;

		static IDType s_EntityCounter;
	};

	inline uint32_t Entity::s_EntityCounter = 0;

}

