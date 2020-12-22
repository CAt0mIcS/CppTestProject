#include "pch.h"
#include "Components.h"

#include "ECS\Registry.h"


void SpeedTest()
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



int main()
{
	//SpeedTest();

	ECS::Registry registry;
	ECS::Entity e = registry.Create();
	ECS::Entity e2 = registry.Create();
	registry.Emplace<TagComponent>(e, "This is the first entities tag component");
	registry.Emplace<TagComponent>(e2, "This is the second entities tag component");
	registry.Emplace<TransformComponent>(e, 3.33f);
	registry.Emplace<TransformComponent>(e2, 4.44f);

	auto view = registry.View<TagComponent, TransformComponent>();
	auto [transformComponent, tagComponent] = registry.Get<TransformComponent, TagComponent>(e);

	//for (auto entity : view)
	//{
	//	std::cout << registry.Get<TagComponent>(entity).tag << '\n';
	//}

	//entt::registry registry;
	//entt::entity e = registry.create();
	//entt::entity e2 = registry.create();
	//registry.emplace<TransformComponent>(e, 3.333f);
	//registry.emplace<TagComponent>(e, "This is a tag");
	//registry.emplace<TransformComponent>(e2, 4.444f);
	//registry.emplace<TagComponent>(e2, "This is a 2 tag");

	//auto view = registry.view<TransformComponent, TagComponent>();
	//auto [transformComponent, tagComponent] = registry.get<TransformComponent, TagComponent>(e);

	//for (entt::entity e : view)
	//{
	//	TransformComponent& transformComponent = registry.get<TransformComponent>(e);
	//	TagComponent& tagComponent = registry.get<TagComponent>(e);
	//}

}