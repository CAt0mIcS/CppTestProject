#include "pch.h"


namespace sstd
{
	template<typename T, size_t S>
	class Array
	{
	public:
		Array() : nextIdx(0) {}
		Array(std::initializer_list<T> initList)
			: nextIdx(0)
		{
			for (auto e : initList)
			{
				this->Add(e);
			}
		}

		void Add(const T& elem)
		{
			m_Arr[nextIdx] = elem;
			++nextIdx;
		}

		constexpr size_t Size() const { return S; }

		T& operator[](const size_t index) { return m_Arr[index]; }
		const T& operator[](const size_t index) const { return m_Arr[index]; }

		const Array operator=(const Array& other)
		{
			for (unsigned int i = 0; i < S; ++i)
			{
				other[i] = m_Arr[i];
			}
		}

		T* Data() { return m_Arr; }
		const T* Data() const { return m_Arr; }

		//unsigned int begin()
		//{
		//	return 0;
		//}
		//unsigned int end()
		//{
		//	return this->Size();
		//}

	private:
		T m_Arr[S];
		int nextIdx;
	};
}


int main()
{
	//std::vector<int> vec{1, 4, 6, 2, 7, 3, 8, 3, 65, 23, 4};

	//for (int a : vec)
	//{
	//	std::cout << a << '\n';
	//}
	
	sstd::Array<int, 10> arr{ 1, 4, 2, 8, 3, 5, 5, 8, 2, 4 };

	for (unsigned int i = 0; i < arr.Size(); ++i)
	{
		std::cout << arr[i] << '\n';
	}

}


