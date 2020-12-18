#pragma once

#include "Entity.h"
#include <vector>


namespace ECS
{
	class SparseSet
	{
	public:
		Entity Search(Entity entity)
		{
			if (m_Sparse[entity] < n && m_Packed[m_Sparse[entity]] == entity)
				return m_Sparse[entity];

			assert(false && "Unable to find entity");
		}

		void Insert(Entity entity)
		{
			m_Packed.emplace_back(entity); // at index n
			m_Sparse.emplace_back(n); // at index entity

			++n;
		}

		void Remove(Entity entity)
		{
			Entity temp = m_Packed[n - 1];
			m_Packed[m_Sparse[entity]] = temp;
			m_Sparse[temp] = m_Sparse[entity];

			--n;
		}

	private:
		std::vector<Entity> m_Packed;
		std::vector<uint32_t> m_Sparse;
		size_t n = 0;
	};
}


