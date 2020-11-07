#include "utpch.h"
#include "TypeConvert.h"


namespace At0::Util
{
	std::wstring MultiByteToWideChar(const std::string_view str)
	{
		auto size = ::MultiByteToWideChar(CP_UTF8, MB_COMPOSITE, str.data(), -1, nullptr, 0);

		std::wstring buff;
		buff.resize(size);
		::MultiByteToWideChar(CP_UTF8, MB_COMPOSITE, str.data(), -1, buff.data(), size);

		return buff;
	}

	std::string WideCharToMultiByte(const std::wstring_view str)
	{
		auto size = ::WideCharToMultiByte(CP_UTF8, WC_COMPOSITECHECK, str.data(), -1, nullptr, 0, NULL, NULL);

		std::string buff;
		buff.resize(size);
		::WideCharToMultiByte(CP_UTF8, WC_COMPOSITECHECK, str.data(), -1, buff.data(), size, NULL, NULL);

		return buff;
	}
	
	RECT TransformationToRect(const DirectX::XMMATRIX& matrix)
	{
		return {};
	}
}



