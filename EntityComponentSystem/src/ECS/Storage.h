#pragma once

#include "Internal.h"
#include "SparseSet.h"


namespace ECS
{
	template<typename Component>
	struct Storage : SparseSet
	{
	public:
		template<typename... Args>
		void Emplace(Entity& entity, Args&&... args)
		{
			if constexpr (std::is_aggregate_v<Component>)
				m_Instances.push_back(Component{ std::forward<Args>(args)... });
			else
				m_Instances.emplace_back(std::forward<Args>(args)...);

			Internal::SetEntityComponentIndex(entity, (uint16_t)m_Instances.size() - 1);

			SparseSet::Emplace(entity);
		}

		const Component& Get(Entity entity) const
		{
			uint16_t compID = Internal::GetEntityComponentIndex(entity);
			return m_Instances[compID];
		}

		Component& Get(Entity entity)
		{
			return const_cast<Component&>(std::as_const(*this).Get(entity));
		}

	private:
		std::vector<Component> m_Instances;
	};
}

