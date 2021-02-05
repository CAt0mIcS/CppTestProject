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
	void Graphics::Create()
	{
		if (!s_Instance)
			s_Instance = std::make_unique<Graphics>();
	}

	Graphics::Graphics()
	{
		m_Instance = std::make_unique<VulkanInstance>();
		m_PhysicalDevice = std::make_unique<PhysicalDevice>(m_Instance.get());
		m_Surface = std::make_unique<Surface>(m_Instance.get(), m_PhysicalDevice.get());
		m_LogicalDevice = std::make_unique<LogicalDevice>(
			m_Instance.get(), m_PhysicalDevice.get(), m_Surface.get());
		m_CommandPool = std::make_unique<CommandPool>(m_LogicalDevice.get());
	}
	Graphics::~Graphics() {}
}  // namespace At0::VulkanTesting
