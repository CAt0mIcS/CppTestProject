#include "pch.h"


int main()
{
	using ScoreMap = std::unordered_map<std::string, int>;
	ScoreMap map;
	map["Cherno"] = 5;
	map["C++"] = 2;

	for (ScoreMap::const_iterator it = map.begin(); it != map.end(); ++it)
	{
		auto& key = it->first;
		auto& value = it->second;
		std::cout << key << " = " << value << '\n';
	}

	std::cout << "\n\n";

	//kv = key-value (std::pair)
	for (auto kv : map)
	{
		auto& key = kv.first;
		auto& value = kv.second;
		std::cout << key << " = " << value << '\n';
	}

	std::cout << "\n\n";

	for (auto [key, value] : map)
	{
		std::cout << key << " = " << value << '\n';
	}
}
