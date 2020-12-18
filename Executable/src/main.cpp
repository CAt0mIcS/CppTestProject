#include "pch.h"
#include "Components.h"

#include "ECS\Registry.h"


int main()
{
	ECS::SparseSet sparseSet;


	std::cout << "ECS: \n";
	auto tStart = std::chrono::high_resolution_clock::now();

	static constexpr uint32_t arrSize = 40000;
	ECS::Entity entities[arrSize];
	for (uint32_t i = 0; i < arrSize; ++i)
	{
		entities[i].Add<TransformComponent>(453.43f);
		std::ostringstream oss;
		oss << "This is Entity " << i;
		entities[i].Add<TagComponent>(oss.str());

		sparseSet.Insert(entities[i]);
	}

	for (uint32_t i = 0; i < arrSize; ++i)
	{
		auto e = sparseSet.Search(entities[i]);
		if (e != entities[i])
			assert(false && "Found entity didn't match searched one");
	}
}