#include "pch.h"
#include "Sampler.h"

#include "Graphics/Graphics.h"
#include "Graphics/Vulkan/PhysicalDevice.h"
#include "Graphics/Vulkan/LogicalDevice.h"


namespace At0::VulkanTesting
{
	Sampler::Sampler()
	{
		VkSamplerCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		createInfo.flags = 0;
		createInfo.magFilter = VK_FILTER_LINEAR;
		createInfo.minFilter = VK_FILTER_LINEAR;
		createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		createInfo.mipLodBias = 0.0f;
		createInfo.anisotropyEnable = VK_TRUE;

		VkPhysicalDeviceProperties deviceProps =
			Graphics::Get().GetPhysicalDevice().GetProperties();

		createInfo.maxAnisotropy = deviceProps.limits.maxSamplerAnisotropy;
		createInfo.compareEnable = VK_FALSE;
		createInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		createInfo.minLod = 0.0f;
		createInfo.maxLod = 0.0f;
		createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		createInfo.unnormalizedCoordinates = VK_FALSE;

		RAY_VK_THROW_FAILED(
			vkCreateSampler(Graphics::Get().GetLogicalDevice(), &createInfo, nullptr, &m_Sampler),
			"Failed to create sampler");
	}

	Sampler::~Sampler()
	{
		vkDestroySampler(Graphics::Get().GetLogicalDevice(), m_Sampler, nullptr);
	}
}  // namespace At0::VulkanTesting
