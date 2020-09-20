#pragma once

#include <iostream>

namespace sstd
{
	template<typename T>
	class Vector
	{
	public:
		Vector()
			: m_Capacity(0), m_Size(m_Capacity), m_Data(new T[m_Capacity])
		{
			reserve(2);
		}

		~Vector()
		{
			Clear();
			::operator delete(m_Data, m_Capacity * sizeof(T));
		}

		constexpr size_t Size() const { return m_Size; }

		//void emplace_back(const T& data)
		//{
		//	++m_Size;
		//	
		//	if (m_Size > m_Capacity)
		//	{
		//		++m_Capacity;

		//		void* addr = malloc(m_Capacity * sizeof(T));

		//		//memcpy(addr, m_Data, sizeof(m_Data) * m_Size);
		//		memmove(addr, m_Data, sizeof(m_Data) * m_Size);

		//		delete[] m_Data;
		//		m_Data = (T*)addr;
		//	}
		//	
		//	m_Data[m_Size - 1] = data;			
		//}

		//void reserve(const size_t size)
		//{
		//	m_Capacity += size;
		//	
		//	void* addr = malloc(m_Capacity * sizeof(T));
		//	//memcpy(addr, m_Data, sizeof(m_Data) * m_Size);
		//	memmove(addr, m_Data, sizeof(m_Data) * m_Size);

		//	delete[] m_Data;
		//	m_Data = (T*)addr;
		//}

		void PopBack()
		{
			if (m_Size > 0)
			{
				--m_Size;
				m_Data[m_Size].~T();
			}
		}

		void PushBack(const T& data)
		{
			if (m_Size >= m_Capacity)
				reserve(m_Capacity + 10);

			m_Data[m_Size] = data;
			++m_Size;
		}

		void PushBack(T&& data)
		{
			if (m_Size >= m_Capacity)
				reserve(m_Capacity + 10);

			m_Data[m_Size] = std::move(data);
			++m_Size;
		}

		void Clear()
		{
			for (size_t i = 0; i < m_Size; ++i)
				m_Data[i].~T();
			m_Size = 0;
		}

		template<typename... Args>
		T& EmplaceBack(Args&&... args)
		{
			if (m_Size >= m_Capacity)
				reserve(m_Capacity + 10);

			// Placement new operator
			new(&m_Data[m_Size]) T(std::forward<Args>(args)...);
			//m_Data[m_Size] = T(std::forward<Args>(args)...);
			return m_Data[++m_Size];
		}

		void reserve(const size_t newCap)
		{
			// Problem: PopBack deletes m_MemoryBlock in Vector3, Clear calls it again (trying to delete deleted element)
			// Solution: Use versions that don't call the constructor of Vector3 (malloc)
			T* newBlock = (T*)::operator new(newCap * sizeof(T));

			if (newCap < m_Size)
				m_Size = newCap;

			for (size_t i = 0; i < m_Size; ++i)
				newBlock[i] = std::move(m_Data[i]);
		
			for (size_t i = 0; i < m_Size; ++i)
				m_Data[i].~T();

			::operator delete(m_Data, m_Capacity * sizeof(T));
			m_Data = newBlock;
			m_Capacity = newCap;
		}

		const T* GetData() const { return m_Data; }

		const T& operator[](const size_t idx)
		{
			return m_Data[idx];
		}

	private:
		T* m_Data;
		size_t m_Capacity;
		size_t m_Size;
	};
}
