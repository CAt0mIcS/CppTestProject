#ifndef RLU_RANDOM_H
#define RLU_RANDOM_H

#include <string>
#include "RlUBase.h"


namespace At0::Util
{
	/// <summary>
	/// std::string version of the bellow function overloads
	/// </summary>
	RLU_API std::string GenerateRandomTokenA(uint16_t length);
	
	/// <summary>
	/// std::wstring version of the bellow function overloads
	/// </summary>
	RLU_API std::wstring GenerateRandomTokenW(uint16_t length);

	/// <summary>
	/// Generates a random string with length
	/// </summary>
	/// <param name="length">Is the length of the generated string</param>
	/// <returns>The randomly generated string</returns>
	template<typename T>
	T GenerateRandomToken(uint16_t length, typename std::enable_if_t<std::is_same_v<T, std::wstring>>* = 0)
	{
		return GenerateRandomTokenW(length);
	}

	/// <summary>
	/// Generates a random string with length
	/// </summary>
	/// <param name="length">Is the length of the generated string</param>
	/// <returns>The randomly generated string</returns>
	template<typename T>
	T GenerateRandomToken(uint16_t length, typename std::enable_if_t<std::is_same_v<T, std::string>>* = 0)
	{
		return GenerateRandomTokenA(length);
	}
}


#endif // RLU_RANDOM_H
