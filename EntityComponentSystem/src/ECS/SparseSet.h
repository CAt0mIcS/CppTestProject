#pragma once

#include <vector>
#include "Internal.h"
#include <assert.h>


namespace ECS
{
	struct SparseSet
	{
	public:
		class Iterator
		{
		public:
			Iterator(const std::vector<Entity>& packed, uint32_t index)
				: m_Packed(packed), m_Idx(index) {}

			Iterator& operator++()
			{
				return --m_Idx, *this;
			}

			Iterator& operator--()
			{
				return ++m_Idx, *this;
			}

			Iterator& operator--(int)
			{
				Iterator orig = *this;
				return operator--(), orig;
			}

			Iterator& operator+=(uint32_t value)
			{
				m_Idx -= value;
				return *this;
			}

			Iterator& operator-=(uint32_t value)
			{
				m_Idx += value;
				return *this;
			}

			Iterator operator+(uint32_t value) const
			{
				Iterator copy = *this;
				return (copy += value);
			}

			Iterator operator-(uint32_t value) const
			{
				Iterator copy = *this;
				return (copy -= value);
			}

			const Entity& operator[](size_t idx) const
			{
				const size_t pos = m_Idx - idx - 1;
				return m_Packed[pos];
			}

			bool operator==(const Iterator& other) const
			{
				return other.m_Idx == m_Idx;
			}

			bool operator!=(const Iterator& other) const
			{
				return !(*this == other);
			}

			bool operator<(const Iterator& other) const
			{
				return m_Idx > other.m_Idx;
			}

			bool operator>(const Iterator& other) const
			{
				return m_Idx < other.m_Idx;
			}

			bool operator<=(const Iterator& other) const
			{
				return !(*this > other);
			}

			bool operator>=(const Iterator& other) const
			{
				return !(*this < other);
			}

			const Entity* operator->() const
			{
				const size_t pos = m_Idx - 1;
				return &m_Packed[pos];
			}

			const Entity& operator*() const
			{
				return *operator->();
			}

		private:
			const std::vector<Entity>& m_Packed;
			size_t m_Idx;
		};

	public:
		Entity Search(Entity entity)
		{
			if (m_Sparse[entity] < n && m_Packed[m_Sparse[entity]] == entity)
				return m_Sparse[entity];

			assert(false && "Unable to find entity");
		}

		void Insert(Entity entity)
		{
			m_Sparse.emplace_back(n); // at index entity
			m_Packed.emplace_back(entity); // at index n

			++n;
		}

		void Remove(Entity entity)
		{
			Entity temp = m_Packed[n - 1];
			m_Packed[m_Sparse[entity]] = temp;
			m_Sparse[temp] = m_Sparse[entity];

			--n;
		}

		size_t Index(Entity entity) const
		{
			return m_Sparse[entity];
		}

		size_t Size() const
		{
			return m_Packed.size();
		}

		Iterator begin() const
		{
			return Iterator{ m_Packed, m_Packed.size() };
		}

		Iterator end() const
		{
			return Iterator{ m_Packed, {} };
		}

	private:
		std::vector<Entity> m_Packed;
		std::vector<uint32_t> m_Sparse;
		uint32_t n = 0;
	};
}


