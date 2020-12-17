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
	TagComponent tag = registry.Emplace<TagComponent>(entity, "This is a tag component");
	std::cout << tag.tag << '\n';
}