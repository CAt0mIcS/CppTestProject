#include "pch.h"
#include "Components.h"

#include "ECS\Registry.h"

template<size_t arrSize = 10000000>
void SpeedTest(uint32_t num = 1)
{
	float avgCreationTimeECS = 0.0f;
	float avgCreationTimeENTT = 0.0f;
	float avgGetTimeECS = 0.0f;
	float avgGetTimeENTT = 0.0f;
	for (size_t i = 0; i <= num; ++i)
	{
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
			avgCreationTimeECS += (end - start).count() / 1000.0f / 1000.0f;
			std::cout << "Creation: " << (end - start).count() / 1000.0f / 1000.0f << "ms\n";

			start = std::chrono::high_resolution_clock::now();
			for (uint32_t i = 0; i < arrSize; ++i)
			{
				TransformComponent& tForm = registry.Get<TransformComponent>(entities[i]);
			}
			end = std::chrono::high_resolution_clock::now();
			avgGetTimeECS += (end - start).count() / 1000.0f / 1000.0f;
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
			avgCreationTimeENTT += (end - start).count() / 1000.0f / 1000.0f;
			std::cout << "Creation: " << (end - start).count() / 1000.0f / 1000.0f << "ms\n";

			start = std::chrono::high_resolution_clock::now();
			for (uint32_t i = 0; i < arrSize; ++i)
			{
				TransformComponent& tForm = registry.get<TransformComponent>(entities[i]);
			}
			end = std::chrono::high_resolution_clock::now();
			avgGetTimeENTT += (end - start).count() / 1000.0f / 1000.0f;
			std::cout << "Get: " << (end - start).count() / 1000.0f / 1000.0f << "ms\n";
		}


		std::cout << "\n\n=========================== NEW TEST =========================== \n";
	}

	std::cout << "\n\n=========================== RESULT =========================== \n";

	std::cout << "ECS: \n\tAverage Creation Time: " << avgCreationTimeECS / num << "\n\tAverage Get Time: " << avgGetTimeECS / num << "\n";
	std::cout << "ENTT: \n\tAverage Creation Time: " << avgCreationTimeENTT / num << "\n\tAverage Get Time: " << avgGetTimeENTT / num << "\n";
}



int main()
{
	SpeedTest<5000000>(10);


	//{
	//	entt::registry registry;
	//	entt::entity e = registry.create();
	//	entt::entity e2 = registry.create();
	//	registry.emplace<TransformComponent>(e2, 3.33f);
	//	registry.emplace<TagComponent>(e, "1st Tag");
	//}
	//
	//{
	//	ECS::Registry registry;
	//	ECS::Entity e = registry.Create();
	//	ECS::Entity e2 = registry.Create();
	//	registry.Emplace<TransformComponent>(e2, 3.33f);
	//	registry.Emplace<TagComponent>(e, "1st Tag");

	//	auto view = registry.View<TransformComponent, TagComponent>();
	//	for (ECS::Entity e : view)
	//	{
	//		std::cout << registry.Get<TagComponent>(e).tag << '\n';
	//	}
	//}

}