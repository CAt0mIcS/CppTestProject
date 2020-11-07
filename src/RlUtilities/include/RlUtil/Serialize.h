#ifndef RLU_SERIALIZE_H
#define RLU_SERIALIZE_H

#include <string>
#include "TypeConvert.h"

#include "RlUBase.h"


namespace At0::Util
{
	template<typename T>
	using BaseRefType = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

	/// <summary>
	/// Takes a correct string possibly containing "{0}", "{1}", ... and fills these in with the corresponding argument
	/// </summary>
	/// <typeparam name="...Args">Is any list of arguments that have a output operator defined</typeparam>
	/// <param name="serializedStr">Is the string which should be serialized</param>
	/// <param name="...args">Are the arguments to insert into the string</param>
	/// <returns>The serialized string</returns>
	template<typename... Args>
	std::string SerializeString(std::string serializedStr, Args&&... args)
	{
		int argCount = 0;
		(SerializeStringArg(serializedStr, args, argCount), ...);
		return serializedStr;
	}

	/// <summary>
	/// Inserts one argument into the string, this function overload is called when the argument type is a std::wstring
	/// </summary>
	/// <typeparam name="T">Is any list of arguments that have a output operator defined</typeparam>
	/// <param name="message">Is the string to insert the argument into</param>
	/// <param name="arg">Is the argument to insert</param>
	/// <param name="argCount">Is the current number where the argument will be inserted ({0} = 0, {3} = 3, ...)</param>
	template<typename T>
	void SerializeStringArg(std::string& message, T&& arg, int& argCount,
		typename std::enable_if_t<std::is_same_v<BaseRefType<T>, std::wstring>>* = 0
	)
	{
		WideCharSerialize(message, arg, argCount);
	}

	/// <summary>
	/// Inserts one argument into the string, this function overload is called when the argument type is convertible to a std::wstring
	/// </summary>
	/// <typeparam name="T">Is any list of arguments that have a output operator defined</typeparam>
	/// <param name="message">Is the string to insert the argument into</param>
	/// <param name="arg">Is the argument to insert</param>
	/// <param name="argCount">Is the current number where the argument will be inserted ({0} = 0, {3} = 3, ...)</param>
	template<typename T>
	void SerializeStringArg(std::string& message, T&& arg, int& argCount,
		typename std::enable_if_t<std::is_convertible_v<T, std::wstring>>* = 0,
		typename std::enable_if_t<!std::is_same_v<BaseRefType<T>, std::wstring>>* = 0
	)
	{
		WideCharSerialize(message, arg, argCount);
	}

	/// <summary>
	/// Inserts one argument into the string, this function overload is called when the argument type is a std::wstring_view
	/// </summary>
	/// <typeparam name="T">Is any list of arguments that have a output operator defined</typeparam>
	/// <param name="message">Is the string to insert the argument into</param>
	/// <param name="arg">Is the argument to insert</param>
	/// <param name="argCount">Is the current number where the argument will be inserted ({0} = 0, {3} = 3, ...)</param>
	template<typename T>
	void SerializeStringArg(std::string& message, T&& arg, int& argCount,
		typename std::enable_if_t<std::is_same_v<BaseRefType<T>, std::wstring_view>>* = 0
	)
	{
		WideCharSerialize(message, arg, argCount);
	}

	/// <summary>
	/// Inserts one argument into the string, this function is called for any type that is not std::wstring and not castable to std::wstring
	/// </summary>
	/// <typeparam name="T">Is any list of arguments that have a output operator defined</typeparam>
	/// <param name="message">Is the string to insert the argument into</param>
	/// <param name="arg">Is the argument to insert</param>
	/// <param name="argCount">Is the current number where the argument will be inserted ({0} = 0, {3} = 3, ...)</param>
	template<typename T>
	void SerializeStringArg(std::string& message, T&& arg, int& argCount,
		typename std::enable_if_t<!std::is_same_v<BaseRefType<T>, std::wstring>>* = 0,
		typename std::enable_if_t<!std::is_convertible_v<T, std::wstring>>* = 0,
		typename std::enable_if_t<!std::is_same_v<BaseRefType<T>, std::wstring_view>>* = 0
	)
	{
		MultiByteSerialize(message, arg, argCount);
	}

	/// <summary>
	/// Inserts the argument into the string
	/// </summary>
	/// <typeparam name="T">Is any argument type which should be serialized using wide character strings</typeparam>
	/// <param name="message">Is the string to insert the argument into</param>
	/// <param name="arg">Is the argument to insert</param>
	/// <param name="argCount">Is the current number where the argument will be inserted ({0} = 0, {3} = 3, ...)</param>
	template<typename T>
	void WideCharSerialize(std::string& message, T&& arg, int& argCount)
	{
		if (argCount == -1)
			return;

		std::wostringstream oss;
		oss << arg;

		const std::string toFind = "{" + std::to_string(argCount) + "}";
		size_t foundIdx = message.find(toFind);

		if (foundIdx == std::string::npos)
		{
			argCount = -1;
			return;
		}

		message.replace(message.begin() + foundIdx, message.begin() + foundIdx + std::size(toFind), WideCharToMultiByte(oss.str()));
		++argCount;
	}

	/// <summary>
	/// Inserts the argument into the string
	/// </summary>
	/// <typeparam name="T">Is any argument type which should be serialized using multi byte strings</typeparam>
	/// <param name="message">Is the string to insert the argument into</param>
	/// <param name="arg">Is the argument to insert</param>
	/// <param name="argCount">Is the current number where the argument will be inserted ({0} = 0, {3} = 3, ...)</param>
	template<typename T>
	void MultiByteSerialize(std::string& message, T&& arg, int& argCount)
	{
		if (argCount == -1)
			return;

		std::ostringstream oss;
		oss << arg;

		const std::string toFind = "{" + std::to_string(argCount) + "}";
		size_t foundIdx = message.find(toFind);

		if (foundIdx == std::string::npos)
		{
			argCount = -1;
			return;
		}

		message.replace(message.begin() + foundIdx, message.begin() + foundIdx + std::size(toFind), oss.str());
		++argCount;
	}
}

#endif // RLU_SERIALIZE_H