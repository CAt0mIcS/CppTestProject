#include "pch.h"
#include "Components.h"

#include "ECS\Entity.h"



int main()
{
	//ECS::Registry registry;
	//registry.Emplace<TagComponent>();
	//registry.Emplace<TransformComponent>();
	//registry.Emplace<SpriteRenderComponent>();
	//registry.Emplace<RenderComponent>();
	//registry.Emplace<NativeScriptingComponent>();

	//if (registry.Has<TagComponent>())
	//{
	//	std::cout << "Component found\n";
	//}
	//else
	//{
	//	std::cout << "Component not found\n";
	//}
	std::cout << "ECS: \n";
	auto tStart = std::chrono::high_resolution_clock::now();

	static constexpr uint32_t arrSize = 50000;
	ECS::Entity entities[arrSize];
	for (uint32_t i = 0; i < arrSize; ++i)
	{
		entities[i].Add<TransformComponent>(453.43f);
		std::ostringstream oss;
		oss << "This is Entity " << i;
		entities[i].Add<TagComponent>(oss.str());
	}

	auto tEnd = std::chrono::high_resolution_clock::now();
	std::cout << "[ECS] Creation finished: " << (tEnd - tStart).count() / 1000.0f / 1000.0f << "ms\n";

	tStart = std::chrono::high_resolution_clock::now();
	for (uint32_t i = 0; i < arrSize; ++i)
	{
		if (entities[i].Has<TransformComponent>())
		{
			auto& tform = entities[i].Get<TransformComponent>();
			tform.x += 0.43f;
		}
	}

	tEnd = std::chrono::high_resolution_clock::now();
	std::cout << "[ECS] GetSet finished: " << std::chrono::duration_cast<std::chrono::milliseconds>(tEnd - tStart).count() << "ms\n";

	////////////////////////////////////////////////////////////////////////////////////
	// ENTT
	////////////////////////////////////////////////////////////////////////////////////
	std::cout << "\n\nENTT: \n";

	tStart = std::chrono::high_resolution_clock::now();

	entt::registry reg;
	entt::entity enttEntities[arrSize];
	for (uint32_t i = 0; i < arrSize; ++i)
	{
		enttEntities[i] = reg.create();
	}

	for (uint32_t i = 0; i < arrSize; ++i)
	{
		reg.emplace<TransformComponent>(enttEntities[i], 453.43f);
		std::ostringstream oss;
		oss << "This is Entity " << i;
		reg.emplace<TagComponent>(enttEntities[i], oss.str());
	}

	tEnd = std::chrono::high_resolution_clock::now();
	std::cout << "[ENTT] Creation finished: " << (tEnd - tStart).count() / 1000.0f / 1000.0f << "ms\n";

	tStart = std::chrono::high_resolution_clock::now();
	for (uint32_t i = 0; i < arrSize; ++i)
	{
		if (reg.has<TransformComponent>(enttEntities[i]))
		{
			auto& tform = reg.get<TransformComponent>(enttEntities[i]);
			tform.x += 0.43f;
		}
	}

	tEnd = std::chrono::high_resolution_clock::now();
	std::cout << "[ENTT] GetSet finished: " << std::chrono::duration_cast<std::chrono::milliseconds>(tEnd - tStart).count() << "ms\n";


}