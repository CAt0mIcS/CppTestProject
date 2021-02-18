#pragma once

#include "Base.h"
#include "Buffer.h"
//#include "Descriptor.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace At0::VulkanTesting
{
	class UniformBuffer : /*public Descriptor, */ public Buffer
	{
	public:
		UniformBuffer(VkDeviceSize size, const void* data = nullptr);

		void Update(const void* newData);

		// WriteDescriptorSet GetWriteDescriptor(
		//	uint32_t binding, VkDescriptorType descriptorType) const override;

		static VkDescriptorSetLayoutBinding GetDescriptorSetLayout(uint32_t binding,
			VkDescriptorType descriptorType, VkShaderStageFlags stage, uint32_t count);
	};
}  // namespace At0::VulkanTesting
