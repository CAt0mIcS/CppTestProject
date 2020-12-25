#pragma once

#include "ComponentStorage.h"
#include <tuple>


namespace ECS
{
	template<typename... Component>
	class View
	{
	private:
		// If the Component (Comp) is const then PoolType will be "const ComponentStorage<std::remove_const_t<Comp>>" else just "ComponentStorage<Comp>"
		template<typename Comp>
		using PoolType = std::conditional_t<std::is_const_v<Comp>, const ComponentStorage<std::remove_const_t<Comp>>, ComponentStorage<Comp>>;

	public:
		class Iterator
		{
		public:
			Iterator(const EntityStorage& candidate, EntityStorage::Iterator curr)
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
			const EntityStorage& m_View;
			EntityStorage::Iterator m_It;
		};

	public:
		View(ComponentStorage<Component>&... component)
			: m_Components{ &component... }
		{

		}

		template<typename... Comp>
		decltype(auto) Get(Entity e)
		{
			if constexpr (sizeof...(Comp) == 0)
			{
				return std::forward_as_tuple(std::get<PoolType<Component>*>(m_Components)->Get(e)...);
			}
			else if constexpr (sizeof...(Comp) == 1)
			{
				return (std::get<PoolType<Comp>*>(m_Components)->Get(e), ...);
			}
			else
			{
				return std::forward_as_tuple(std::get<PoolType<Comp>*>(m_Components)->Get(e)...);
			}
		}

		Iterator begin() const
		{
			const EntityStorage& view = Candidate();
			return Iterator{ view, view.begin() };
		}

		Iterator end() const
		{
			const EntityStorage& view = Candidate();
			return Iterator{ view, view.end() };
		}

	private:
		const EntityStorage& Candidate() const
		{
			return *std::min({ static_cast<const EntityStorage*>(std::get<PoolType<Component>*>(m_Components))... }, [](const EntityStorage* lhs, const EntityStorage* rhs)
				{
					return lhs->Size() < rhs->Size();
				}
			);
		}

	private:
		std::tuple<PoolType<Component>*...> m_Components;
	};
}