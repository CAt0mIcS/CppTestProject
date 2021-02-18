#pragma once

#include "Base.h"

#include <unordered_map>
#include <memory>

#include "Graphics/Vulkan/Bindable.h"
#include "Utils/RLogger.h"


namespace At0::VulkanTesting
{
	class Codex
	{
	public:
		template<typename T, typename... Args>
		static std::shared_ptr<T> Resolve(Args&&... args)
		{
			static_assert(std::is_base_of<Bindable, T>::value,
				"Template argument must be derived from Bindable");
			return Get().InternalResolve<T>(std::forward<Args>(args)...);
		}

		static void Shutdown() { Get().InternalShutdown(); }

	private:
		static Codex& Get()
		{
			static Codex codex;
			return codex;
		}

		template<typename T, typename... Args>
		std::shared_ptr<T> InternalResolve(Args&&... args)
		{
			std::string uniqueID = T::GetUID(std::forward<Args>(args)...);
			auto it = m_BindableMap.find(uniqueID);

			// Key does not exist, create bindable
			if (it == m_BindableMap.end())
			{
				m_BindableMap[uniqueID] = std::make_shared<T>(std::forward<Args>(args)...);
				return std::static_pointer_cast<T>(m_BindableMap[uniqueID]);
			}
			// Key exists, return it
			else
			{
				return std::static_pointer_cast<T>(it->second);
			}
		}

		void InternalShutdown() { m_BindableMap.clear(); }

	private:
		std::unordered_map<std::string, std::shared_ptr<Bindable>> m_BindableMap;
	};
}  // namespace At0::VulkanTesting
