#pragma once

#include "Internal.h"
#include "EntityStorage.h"


namespace ECS
{
	template<typename Component>
	struct ComponentStorage : EntityStorage
	{
	public:
		template<typename... Args>
		decltype(auto) Emplace(Entity entity, Args&&... args)
		{
			if constexpr (std::is_aggregate_v<Component>)
				m_Instances.push_back(Component{ std::forward<Args>(args)... });
			else
				m_Instances.emplace_back(std::forward<Args>(args)...);

			EntityStorage::Emplace(entity, (IndexType)m_Instances.size() - 1);

			if constexpr (!std::is_empty_v<Component>)
				return m_Instances.back();
		}

		const Component& Get(Entity entity) const
		{
			return m_Instances[EntityStorage::Index(entity)];
		}

		Component& Get(Entity entity)
		{
			return const_cast<Component&>(std::as_const(*this).Get(entity));
		}

	private:
		std::vector<Component> m_Instances;
	};
}

