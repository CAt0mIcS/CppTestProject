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
	float avgViewTimeECS = 0.0f;
	float avgViewTimeENTT = 0.0f;
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
				registry.Emplace<TagComponent>(entities[i], "Tag " + std::to_string(i));
				registry.Emplace<SpriteRenderComponent>(entities[i], 332);
				registry.Emplace<RenderComponent>(entities[i], 32);
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

			start = std::chrono::high_resolution_clock::now();
			auto view = registry.View<TransformComponent, TagComponent, SpriteRenderComponent>();
			for (auto entity : view)
			{
				TransformComponent& tform = registry.Get<TransformComponent>(entity);
				TagComponent& tag = registry.Get<TagComponent>(entity);
				SpriteRenderComponent& spriteRender = registry.Get<SpriteRenderComponent>(entity);
			}
			end = std::chrono::high_resolution_clock::now();
			avgViewTimeECS += (end - start).count() / 1000.0f / 1000.0f;
			std::cout << "View: " << (end - start).count() / 1000.0f / 1000.0f << "ms\n";
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
				registry.emplace<TagComponent>(entities[i], "Tag " + std::to_string(i));
				registry.emplace<SpriteRenderComponent>(entities[i], 332);
				registry.emplace<RenderComponent>(entities[i], 32);
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

			start = std::chrono::high_resolution_clock::now();
			auto view = registry.view<TransformComponent, TagComponent, SpriteRenderComponent>();
			for (auto entity : view)
			{
				TransformComponent& tform = registry.get<TransformComponent>(entity);
				TagComponent& tag = registry.get<TagComponent>(entity);
				SpriteRenderComponent& spriteRender = registry.get<SpriteRenderComponent>(entity);
			}
			end = std::chrono::high_resolution_clock::now();
			avgViewTimeENTT += (end - start).count() / 1000.0f / 1000.0f;
			std::cout << "View: " << (end - start).count() / 1000.0f / 1000.0f << "ms\n";

		}


		std::cout << "\n\n=========================== NEW TEST =========================== \n";
	}

	std::cout << "\n\n=========================== RESULT =========================== \n";

	std::cout << "ECS: \n\tAverage Creation Time: " << avgCreationTimeECS / num;
	std::cout << "\n\tAverage Get Time: " << avgGetTimeECS / num;
	std::cout << "\n\tAverage View Time: " << avgViewTimeECS / num;

	std::cout << '\n';

	std::cout << "ENTT: \n\tAverage Creation Time: " << avgCreationTimeENTT / num;
	std::cout << "\n\tAverage Get Time: " << avgGetTimeENTT / num;
	std::cout << "\n\tAverage View Time: " << avgViewTimeENTT / num;
}



int main()
{
	static constexpr uint64_t arrSize = 10000000;
	//SpeedTest<arrSize>(10);

	{
		ECS::Registry registry;
		ECS::Entity* entities = new ECS::Entity[arrSize];

		for (uint32_t i = 0; i < arrSize; ++i)
		{
			entities[i] = registry.Create();
			registry.Emplace<TransformComponent>(entities[i], 32.23f);
			registry.Emplace<SpriteRenderComponent>(entities[i], 132432);
		}

		auto start = std::chrono::high_resolution_clock::now();
		for (uint32_t i = 0; i < arrSize; i += 10)
		{
			registry.Destroy(entities[i]);
		}
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "ECS: Destroy: " << (end - start).count() / 1000.0f / 1000.0f << "ms\n";

		start = std::chrono::high_resolution_clock::now();
		for (uint32_t i = 0; i < arrSize; ++i)
		{
			if (registry.Has<TransformComponent, SpriteRenderComponent>(entities[i]))
			{
				auto [transform, render] = registry.Get<TransformComponent, SpriteRenderComponent>(entities[i]);
			}

			if (registry.Has<RenderComponent>(entities[i]))
			{
				auto& render = registry.Get<RenderComponent>(entities[i]);
			}
		}
		end = std::chrono::high_resolution_clock::now();
		std::cout << "ECS: Get&Has: " << (end - start).count() / 1000.0f / 1000.0f << "ms\n";
	}


	{
		entt::registry registry;
		entt::entity* entities = new entt::entity[arrSize];

		for (uint32_t i = 0; i < arrSize; ++i)
		{
			entities[i] = registry.create();
			registry.emplace<TransformComponent>(entities[i], 32.23f);
			registry.emplace<SpriteRenderComponent>(entities[i], 132432);
		}

		auto start = std::chrono::high_resolution_clock::now();
		for (uint32_t i = 0; i < arrSize; i += 10)
		{
			registry.destroy(entities[i]);
		}
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "ECS: Destroy: " << (end - start).count() / 1000.0f / 1000.0f << "ms\n";

		start = std::chrono::high_resolution_clock::now();
		for (uint32_t i = 0; i < arrSize; ++i)
		{
			if (registry.has<TransformComponent, SpriteRenderComponent>(entities[i]))
			{
				auto [transform, render] = registry.get<TransformComponent, SpriteRenderComponent>(entities[i]);
			}

			if (registry.has<RenderComponent>(entities[i]))
			{
				auto& render = registry.get<RenderComponent>(entities[i]);
			}
		}
		end = std::chrono::high_resolution_clock::now();
		std::cout << "ENTT: Get&Has: " << (end - start).count() / 1000.0f / 1000.0f << "ms\n";
	}
}