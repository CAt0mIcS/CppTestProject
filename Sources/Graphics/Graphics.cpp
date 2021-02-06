#include "Graphics.h"
#include "../Utils/RLogger.h"
#include "../Utils/RException.h"
#include "../Utils/RAssert.h"
#include "VulkanInstance.h"
#include "PhysicalDevice.h"
#include "Surface.h"
#include "LogicalDevice.h"
#include "Commands/CommandPool.h"

#include <vulkan/vulkan.h>


namespace At0::VulkanTesting
{
	Graphics::Graphics()
	{
		s_Instance = this;

		m_Instance = std::make_unique<VulkanInstance>();
		m_Surface = std::make_unique<Surface>();
		m_PhysicalDevice = std::make_unique<PhysicalDevice>();
		m_LogicalDevice = std::make_unique<LogicalDevice>();
		m_Swapchain = std::make_unique<Swapchain>();
	}

	Graphics::~Graphics()
	{
		// not production level code here...
		if (s_Instance)
		{
			delete s_Instance;
			s_Instance = nullptr;
		}

		m_Swapchain.reset();
		m_LogicalDevice.reset();
		m_Surface.reset();
		m_Instance.reset();
	}

	Graphics& Graphics::Get()
	{
		if (!s_Instance)
			new Graphics();

		return *s_Instance;
	}
}  // namespace At0::VulkanTesting
