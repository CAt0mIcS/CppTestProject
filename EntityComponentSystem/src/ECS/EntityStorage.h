#pragma once

#include "Internal.h"
#include <vector>
#include <assert.h>



namespace At0::ECS
{
	class EntityStorage
	{
	private:
		struct MappedComponentIndex
		{
			MappedComponentIndex(Entity e, uint32_t idx)
				: IndexInComponentVector(idx), Entity(e)
			{
			}

			uint32_t IndexInComponentVector;
			Entity Entity;
		};

	public:
		void Emplace(Entity e, uint32_t compIdx)
		{
			m_Entities.emplace_back(e, compIdx);
		}

		void Remove(Entity e)
		{
			assert(false && "Missing implementation");
		}

		uint32_t IndexInComponentVector(Entity e) const
		{
			return m_Entities[e].IndexInComponentVector;
		}

		bool Contains(Entity e) const
		{
			return m_Entities.size() > e;
		}

	private:
		// ECS_TODO: Deleted entities shouldn't be erased from the vector (all elements behind need to be moved to fill the gap)
		std::vector<MappedComponentIndex> m_Entities;
	};
}
