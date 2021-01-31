#include "Graphics.h"
#include "../Utils/RLogger.h"
#include "../Utils/RException.h"
#include "../Utils/RAssert.h"

#include <vulkan/vulkan.h>


namespace At0::VulkanTesting
{
	void Graphics::Create()
	{
		if (!s_Instance)
			s_Instance = std::make_unique<Graphics>();
	}
}  // namespace At0::VulkanTesting