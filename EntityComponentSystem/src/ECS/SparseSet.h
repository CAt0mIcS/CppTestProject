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
			Iterator(const std::vector<Entity>& packed, size_t index)
				: m_Packed(packed), m_Idx(index) {}

			Iterator& operator++()
			{
				return ++m_Idx, *this;
			}

			Iterator& operator--()
			{
				return --m_Idx, *this;
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
				const size_t pos = m_Idx;
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
		size_t Size() const
		{
			return m_Entities.size();
		}

		Iterator begin() const
		{
			return Iterator{ m_Entities, 0 };
		}

		Iterator end() const
		{
			return Iterator{ m_Entities, m_Entities.size() };
		}

		void Emplace(Entity entity)
		{
			m_Entities.emplace_back(entity);
		}

	private:
		// All entities which have the Component
		std::vector<Entity> m_Entities;
	};
}


