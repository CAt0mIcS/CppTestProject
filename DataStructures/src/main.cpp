#include <iostream>

#include <vector>
#include <array>

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
	std::array<int, 10> arr;
	sstd::Array<int, 10> vec;
	vec[0] = 2;
	vec[1] = 43;
	vec[2] = 2;
	vec[3] = 5;
	vec[4] = 65;
	vec[5] = 8;
	vec[6] = 3;
	vec[7] = 2;
	vec[8] = 77;
	vec[9] = 04;

	for (int i : vec)
	{
		std::cout << i << '\n';
	}

}

