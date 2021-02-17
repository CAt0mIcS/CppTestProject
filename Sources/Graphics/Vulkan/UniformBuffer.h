#pragma once

#include "Buffer.h"
#include "Bindable.h"
#include "Descriptor.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace At0::VulkanTesting
{
	namespace Constants
	{
		inline constexpr const char* MVPUniformBufferName = "MVPuBuff";
	}

	class Pipeline;

	struct UniformBufferObject
	{
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};

	class UniformBuffer : public Descriptor, public Buffer
	{
	public:
		UniformBuffer(VkDeviceSize size, const void* data = nullptr);

		void Update(const void* newData);

		WriteDescriptorSet GetWriteDescriptor(
			uint32_t binding, VkDescriptorType descriptorType) const override;

		static VkDescriptorSetLayoutBinding GetDescriptorSetLayout(uint32_t binding,
			VkDescriptorType descriptorType, VkShaderStageFlags stage, uint32_t count);
	};
}  // namespace At0::VulkanTesting
