#pragma once

#include <assert.h>
#include <string>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

#define ECS_EXPECTS(expr, msg) if(!(expr)) _wassert(::ECS::Internal::MultiByteToWideChar(#expr ": " msg).c_str(), ::ECS::Internal::MultiByteToWideChar(__FILE__).c_str(), __LINE__)

namespace ECS::Internal
{
	std::wstring MultiByteToWideChar(const char* str)
	{
		auto size = ::MultiByteToWideChar(CP_UTF8, MB_COMPOSITE, str, -1, nullptr, 0);

		std::wstring buff;
		buff.resize(size);
		::MultiByteToWideChar(CP_UTF8, MB_COMPOSITE, str, -1, buff.data(), size);

		return buff;
	}

	struct TypeIndex
	{
		static uint32_t Next()
		{
			static uint32_t counter = 0;
			return counter++;
		}
	};
}

namespace ECS
{
	template<typename T>
	struct TypeIndex
	{
		static uint32_t Value()
		{
			static const uint32_t value = Internal::TypeIndex::Next();
			return value;
		}
	};


	//enum class Entity : uint32_t {};
	//constexpr auto ToIntegral(Entity id)
	//{
	//	return std::underlying_type_t<Entity>(id);
	//}
	using Entity = uint32_t;



	// QUESTION: What?
	template<typename, typename = void>
	struct HasTypeIndex : std::false_type {};
	
	template<typename Type>
	struct HasTypeIndex<Type, std::void_t<decltype(TypeIndex<Type>::Value())>> : std::true_type {};
}