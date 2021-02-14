#include "Surface.h"
#include "Graphics/Graphics.h"
#include "Window.h"
#include "Utils/RLogger.h"
#include "Utils/RException.h"
#include "Utils/RAssert.h"

#include <vulkan/vulkan.h>


namespace At0::VulkanTesting
{
	Surface::Surface()
	{
		Window::Get().CreateSurface(Graphics::Get().GetInstance(), nullptr, &m_Surface);
	}

	Surface::~Surface() { vkDestroySurfaceKHR(Graphics::Get().GetInstance(), m_Surface, nullptr); }
}  // namespace At0::VulkanTesting
