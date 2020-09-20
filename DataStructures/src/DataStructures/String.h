#pragma once


namespace sstd
{

	class String
	{
	public:
		String(const char* data)
			: m_Data((char*)data), m_Size(sizeof(data))		//Bad!
		{

		}

		constexpr int Size() const { return m_Size; }

	private:
		char* m_Data;
		size_t m_Size;
	};

}

