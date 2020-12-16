#pragma once

#include <stdint.h>
#include <vector>
#include <string>

#include <assert.h>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>


#define ECS_EXPECTS(expr, msg) if(!(expr)) _wassert(::ECS::Internal::MultiByteToWideChar(#expr ": " msg).c_str(), ::ECS::Internal::MultiByteToWideChar(__FILE__).c_str(), __LINE__)


namespace ECS
{
	namespace Internal
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

	template<typename T>
	struct TypeIndex
	{
		static uint32_t Value()
		{
			static const uint32_t value = Internal::TypeIndex::Next();
			return value;
		}
	};


	class Registry
	{
	private:
		template<typename Comp>
		struct HasComp
		{
			static bool Value(size_t size)
			{
				return TypeIndex<Comp>::Value() >= size;
			}
		};

	public:
		Registry()
			: m_Components{} 
		{

		}

		template<typename Comp>
		auto Emplace()
		{
			ECS_EXPECTS(m_Components.size() <= TypeIndex<Comp>::Value(), "Component already exists");
			m_Components.push_back(TypeIndex<Comp>::Value());
		}

		template<typename Comp>
		bool Has()
		{
			return (TypeIndex<Comp>::Value() <= m_Components.size());
		}

	private:
		std::vector<uint32_t> m_Components;
	};


}
