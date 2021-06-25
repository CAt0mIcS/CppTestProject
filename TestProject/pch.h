#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <type_traits>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <assert.h>
#include <optional>
#include <vector>
#include <array>
#include <Windows.h>
#include <source_location>

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename... Args>
Scope<T> MakeScope(Args&&... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
Ref<T> MakeRef(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}
