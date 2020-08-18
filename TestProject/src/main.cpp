#include "pch.h"


class Entity
{
public:
	int x, y, z;
	std::string str;

	Entity(int x, int y, int z, const std::string& str)
		:x(x), y(y), z(z), str(str) {}
};


template<typename T>
void AreEqual(T* obj1, T* obj2, const char* message)
{
	char* pObj1 = (char*)obj1;
	char* pObj2 = (char*)obj2;

	for (int i = 0; i < sizeof(T); ++i)
	{
		if ((int)pObj1[i] != (int)pObj2[i])
			__debugbreak();
	}
}


int main()
{
	Entity e (1, 2, 3, "H");
	Entity e2(1, 2, 3, "H");

	AreEqual(&e, &e2, "Messaage);");
}
