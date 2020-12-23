#pragma once

#include "Storage.h"
#include <tuple>


namespace ECS
{
	template<typename... Component>
	class View
	{
	private:
		// If the Component (Comp) is const then PoolType will be "const Storage<std::remove_const_t<Comp>>" else just "Storage<Comp>"
		template<typename Comp>
		using PoolType = std::conditional_t<std::is_const_v<Comp>, const Storage<std::remove_const_t<Comp>>, Storage<Comp>>;

	public:
		class Iterator
		{
		public:
			Iterator(const SparseSet& candidate, SparseSet::Iterator curr)
				: m_View{ candidate }, m_It{ curr } {}

			Iterator operator++()
			{
				++m_It;
				return *this;
			}

			Iterator operator--()
			{
				--m_It;
				return *this;
			}

			Iterator operator--(int)
			{
				Iterator original = *this;
				return operator--(), original;
			}

			bool operator==(const Iterator& other) const
			{
				return other.m_It == m_It;
			}

			bool operator!=(const Iterator& other) const
			{
				return !(*this == other);
			}

			const Entity* operator->() const
			{
				return m_It.operator->();
			}

			const Entity& operator*() const
			{
				return *operator->();
			}

		private:
			const SparseSet& m_View;
			SparseSet::Iterator m_It;
		};

	public:
		View(Storage<Component>&... component)
			: m_Components{ &component... }
		{

		}

		Iterator begin() const
		{
			const SparseSet& view = Candidate();
			return Iterator{ view, view.begin() };
		}

		Iterator end() const
		{
			const SparseSet& view = Candidate();
			return Iterator{ view, view.end() };
		}

	private:
		const SparseSet& Candidate() const
		{
			return *std::min({ static_cast<const SparseSet*>(std::get<PoolType<Component>*>(m_Components))... }, [](const SparseSet* lhs, const SparseSet* rhs)
				{
					return lhs->Size() < rhs->Size();
				}
			);
		}

	private:
		std::tuple<PoolType<Component>*...> m_Components;
	};
}