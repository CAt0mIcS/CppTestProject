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
		s_Instance = std::unique_ptr<Graphics>(this);

		m_Instance = std::make_unique<VulkanInstance>();
		m_Surface = std::make_unique<Surface>();
		m_PhysicalDevice = std::make_unique<PhysicalDevice>();
		m_LogicalDevice = std::make_unique<LogicalDevice>();
		m_Swapchain = std::make_unique<Swapchain>();
	}

	Graphics::~Graphics() {}

	Graphics& Graphics::Get()
	{
		if (!s_Instance)
			std::make_unique<Graphics>();

		return *s_Instance;
	}
}  // namespace At0::VulkanTesting
