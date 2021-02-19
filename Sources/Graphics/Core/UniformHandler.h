#pragma once

#include "Base.h"

#include <vector>
#include <string>

#include "Graphics/Vulkan/UniformBuffer.h"
#include "Graphics/Vulkan/Descriptor.h"


namespace At0::VulkanTesting
{
	class Pipeline;
	class CommandBuffer;

	class UniformHandler
	{
	public:
	private:
		Scope<UniformBuffer> m_UniformBuffer;
		Scope<DescriptorSet> m_DescriptorSet;
	};
}  // namespace At0::VulkanTesting
