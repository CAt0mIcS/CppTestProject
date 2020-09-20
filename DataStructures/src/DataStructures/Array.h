#pragma once


namespace sstd
{
	template<typename T, size_t S>
	class Array
	{
	public:
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

	private:
		int m_Arr[S];
	};
}

