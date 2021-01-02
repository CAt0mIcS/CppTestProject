#pragma once

#include "RInternal.h"

#include <vector>
#include <assert.h>


namespace At0::Ray::ECS
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

		uint32_t IndexInComponentVector(Entity e) const
		{
			return m_Entities[e].IndexInComponentVector;
		}

		bool Contains(Entity e) const
		{
			return m_Entities.size() > e;
		}

		virtual void RemoveEntity(Entity e) = 0;

	protected:
		void SetComponentIndex(Entity e, uint32_t compIdx)
		{
			m_Entities[e].IndexInComponentVector = compIdx;
		}

	private:
		// RAY_TODO: Deleted entities shouldn't be erased from the vector (all elements behind need to be moved to fill the gap)
		std::vector<MappedComponentIndex> m_Entities;
	};
}
