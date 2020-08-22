#include "Encriptor.h"


std::string Encriptor::Encript(const std::string& str)
{
	std::string encStr = "";
	for (char c : str)
	{
		encStr += (c + m_EncKey);
	}
	return encStr;
}

std::string Encriptor::Decript(const std::string& str)
{
	std::string decStr = "";
	for (char c : str)
	{
		decStr += (c - m_EncKey);
	}
	return decStr;
}
