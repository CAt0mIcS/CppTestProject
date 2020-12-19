#include "pch.h"
#include "Components.h"

#include "ECS\Registry.h"


int main()
{
	ECS::Registry registry;
	ECS::Entity entity = registry.Create();
	registry.Emplace<TransformComponent>(entity, 32.432f);

	TransformComponent& tform = registry.Get<TransformComponent>(entity);
	std::cout << tform.x << '\n';
}