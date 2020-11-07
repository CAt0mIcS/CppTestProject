#include "utpch.h"
#include "Random.h"


namespace At0::Util
{
	std::string GenerateRandomTokenA(uint16_t length)
	{
		std::string ret = "";

		static constexpr const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";


		for (uint16_t i = 0; i < length; ++i)
		{
			ret += charset[rand() % (std::size(charset) - 1)];
		}
		return ret;
	}
	
	std::wstring GenerateRandomTokenW(uint16_t length)
	{
		std::wstring ret = L"";

		static constexpr const wchar_t charset[] =
			L"0123456789"
			L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			L"abcdefghijklmnopqrstuvwxyz";


		for (uint16_t i = 0; i < length; ++i)
		{
			ret += charset[rand() % (std::size(charset) - 1)];
		}
		return ret;
	}
}


