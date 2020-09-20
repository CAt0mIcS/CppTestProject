#include <iostream>

#include <vector>

#include "DataStructures/Array.h"
#include "DataStructures/String.h"
#include "DataStructures/Vector.h"

#include "Debug/Timer.h"


struct Vector3
{
	float x = 0.0f, y = 0.0f, z = 0.0f;
	int* m_MemoryBlock = nullptr;

	Vector3() { m_MemoryBlock = new int[5]; }
	Vector3(float scalar) : x(scalar), y(scalar), z(scalar) { m_MemoryBlock = new int[5]; }
	Vector3(float x, float y, float z) : x(x), y(y), z(z) { m_MemoryBlock = new int[5]; }

	Vector3(const Vector3& other) = delete;

	Vector3(Vector3&& other) noexcept
		: x(other.x), y(other.y), z(other.z), m_MemoryBlock(other.m_MemoryBlock)
	{
		other.m_MemoryBlock = nullptr;
		std::cout << "Moved\n";
	}

	~Vector3()
	{
		std::cout << "Destroyed\n";
		delete[] m_MemoryBlock;
	}

	Vector3& operator=(const Vector3& other) = delete;

	Vector3& operator=(Vector3&& other) noexcept
	{
		std::cout << "Moved\n";
		x = other.x;
		y = other.y;
		z = other.z;
		m_MemoryBlock = other.m_MemoryBlock;
		other.m_MemoryBlock = nullptr;
		return *this;
	}

};


int main()
{
	sstd::Vector<Vector3> vec;
	//vec.PushBack(Vector3(1.0f));
	//vec.PushBack(Vector3(2, 3, 4));
	//vec.PushBack(Vector3());

	vec.EmplaceBack(1.0f);
	vec.EmplaceBack(2, 3, 4);
	vec.EmplaceBack(1, 2, 4);
	vec.EmplaceBack();
	vec.PopBack();
	vec.PopBack();
	vec.EmplaceBack(5, 2, 0);
	vec.EmplaceBack(1, 7, 9);
	vec.Clear();
	vec.EmplaceBack(5, 2, 0);
	vec.EmplaceBack(1, 7, 9);
}

