#include "pch.h"
#include "Components.h"

#include <ECS\EntityComponentSystem.h>



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

	ECS::Registry registry;
	ECS::Entity entity = registry.Create();
	TagComponent& tag = registry.Emplace<TagComponent>(entity, "This is the first entity's a tag component");
	TransformComponent& transform = registry.Emplace<TransformComponent>(entity, 3.0f);
	std::cout << tag.tag << '\n';
	std::cout << "TransformComponent: " << transform.x << '\n';

	ECS::Entity entity2 = registry.Create();
	TagComponent& tag2 = registry.Emplace<TagComponent>(entity2, "This is the 2nd entity's tag component");
	TransformComponent& transform2 = registry.Emplace<TransformComponent>(entity2, 6.0f);
	std::cout << tag2.tag << '\n';
	std::cout << "TransformComponent: " << transform2.x << '\n';
}