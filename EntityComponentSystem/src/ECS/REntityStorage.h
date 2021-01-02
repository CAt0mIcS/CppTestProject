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
				: Entity(e), IndexInComponentVector(idx) {}

			Entity Entity;
			uint32_t IndexInComponentVector;
		};

	public:
		void Emplace(Entity e, uint32_t idxInCompVec)
		{
			m_Packed.emplace_back(e, idxInCompVec);
			m_Sparse.emplace_back(m_NumElements);
			++m_NumElements;
		}

		void Delete(Entity e)
		{
			assert(Contains(e), "[EntityStorage::Delete] Entity (ID={0}) not stored.");
			MappedComponentIndex temp = m_Packed.back();
			m_Packed[m_Sparse[e]] = temp;
			m_Sparse[temp.IndexInComponentVector] = m_Sparse[e];

			--m_NumElements;
		}

		void Clear()
		{
			m_NumElements = 0;
		}

		bool Contains(Entity e) const
		{
			return e < m_Sparse.size() && m_Sparse[e] < m_NumElements&& m_Packed[m_Sparse[e]].Entity == e;
		}

		virtual void RemoveEntity(Entity e) = 0;

	protected:
		uint32_t IndexInComponentVector(Entity e) const
		{
			return m_Packed[m_Sparse[e]].IndexInComponentVector;
		}

		void SetComponentIndex(Entity e, uint32_t idx)
		{
			m_Packed[m_Sparse[e]].IndexInComponentVector = idx;
		}

	private:
		std::vector<MappedComponentIndex> m_Packed{};
		std::vector<uint32_t> m_Sparse{};

		uint32_t m_NumElements{ 0 };
	};
}
