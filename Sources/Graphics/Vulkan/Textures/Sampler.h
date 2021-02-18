#pragma once

#include "../Bindable.h"

#include <vulkan/vulkan_core.h>


namespace At0::VulkanTesting
{
	class Sampler
	{
	public:
		Sampler();
		~Sampler();

		operator const VkSampler&() const { return m_Sampler; }

	private:
		VkSampler m_Sampler;
	};
}  // namespace At0::VulkanTesting
