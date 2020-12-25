#pragma once

#include <stdint.h>
#include <type_traits>


namespace ECS
{
	using IndexType = uint32_t;
	using Entity = uint64_t;

	//struct Entity
	//{
	//	uint64_t ID : 48;
	//	uint16_t ComponentIndex : 16;

	//	Entity(uint64_t id) : ID(id), ComponentIndex{} {}
	//	Entity() = default;

	//	bool operator==(const Entity& other) const
	//	{
	//		return ID == other.ID;
	//	}
	//};

	namespace Internal
	{
		uint16_t GetEntityComponentIndex(Entity entity)
		{
			return entity & (entity >> 48);
		}

		void SetEntityComponentIndex(Entity& entity, uint16_t idx)
		{
			entity = entity | (((uint64_t)(idx)) << 48);
		}

		uint64_t GetEntityID(Entity entity)
		{
			return entity & 0x00ffffff;
		}

		void SetEntityID(Entity& entity, uint64_t id)
		{
			entity = entity | id;
		}

		struct ComponentIndex
		{
			static IndexType Next()
			{
				static IndexType idx = 0;
				return idx++;
			}
		};
	}


	template<typename Component>
	struct ComponentIndex
	{
		static IndexType Value()
		{
			static IndexType next = Internal::ComponentIndex::Next();
			return next;
		}
	};

	template<typename Component, typename = void>
	struct ComponentInfo
	{
		static uint32_t ID()
		{
			return ComponentIndex<Component>::Value();
		}
	};

	// If ComponentIndex<Component>::Value() already exists for the component then the type from this function (decltype)
	// will be converted to void and the std::true_type struct will be used because of template specialisation. If the function Value()
	// for the specific component has not been generated, then decltype will fail and the std::false_type struct will be used.

	template<typename, typename = void>
	struct HasComponentIndex : std::false_type {};

	template<typename Component>
	struct HasComponentIndex<Component, std::void_t<decltype(ComponentIndex<Component>::Value())>> : std::true_type {};
}

