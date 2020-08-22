#pragma once

#include <string>


class Encriptor
{
public:
	Encriptor() : m_EncKey(rand() / 11) { }

	std::string Encript(_Inout_ const std::string& str);
	std::string Decript(_Inout_ const std::string& str);

private:
	int m_EncKey;
};

