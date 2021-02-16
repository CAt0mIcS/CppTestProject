﻿#pragma once

#include <vulkan/vulkan.h>

#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <array>
#include <memory>


namespace At0::VulkanTesting
{
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename... Args>
	auto MakeRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	auto MakeScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}  // namespace At0::VulkanTesting
