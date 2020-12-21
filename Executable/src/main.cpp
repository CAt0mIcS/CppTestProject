#include "pch.h"
#include "Components.h"

#include "ECS\Registry.h"



int main()
{
	static constexpr uint32_t arrSize = 10000000;
	std::cout << "=========================== ECS =========================== \n";

	{
		auto start = std::chrono::high_resolution_clock::now();

		ECS::Registry registry;
		ECS::Entity* entities = new ECS::Entity[arrSize];

		for (uint32_t i = 0; i < arrSize; ++i)
		{
			entities[i] = registry.Create();
			registry.Emplace<TransformComponent>(entities[i], 3245.423f);
		}
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "Creation: " << (end - start).count() / 1000.0f / 1000.0f << "ms\n";

		start = std::chrono::high_resolution_clock::now();
		for (uint32_t i = 0; i < arrSize; ++i)
		{
			TransformComponent& tForm = registry.Get<TransformComponent>(entities[i]);
		}
		end = std::chrono::high_resolution_clock::now();
		std::cout << "Get: " << (end - start).count() / 1000.0f / 1000.0f << "ms\n";
	}

	std::cout << "=========================== ENTT =========================== \n";

	{
		auto start = std::chrono::high_resolution_clock::now();

		entt::registry registry;
		entt::entity* entities = new entt::entity[arrSize];

		for (uint32_t i = 0; i < arrSize; ++i)
		{
			entities[i] = registry.create();
			registry.emplace<TransformComponent>(entities[i], 3245.423f);
		}
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "Creation: " << (end - start).count() / 1000.0f / 1000.0f << "ms\n";

		start = std::chrono::high_resolution_clock::now();
		for (uint32_t i = 0; i < arrSize; ++i)
		{
			TransformComponent& tForm = registry.get<TransformComponent>(entities[i]);
		}
		end = std::chrono::high_resolution_clock::now();
		std::cout << "Get: " << (end - start).count() / 1000.0f / 1000.0f << "ms\n";
	}

}