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

	class UniformBuffer : public Buffer, public Bindable
	{
	public:
		UniformBuffer(const Pipeline& pipeline, std::string_view tag = "");

		void Bind(const CommandBuffer& cmdBuff) override;
		void Update(const UniformBufferObject& ubo);

		const DescriptorSet& GetDescriptorSet() const { return m_DescriptorSet; }

		static VkDescriptorSetLayoutBinding GetDescriptorSetLayout(uint32_t binding,
			VkDescriptorType descriptorType, VkShaderStageFlags stage, uint32_t count);

		static std::string GetUID(const Pipeline& pipeline, std::string_view tag = "");

	private:
		DescriptorSet m_DescriptorSet;
	};
}  // namespace At0::VulkanTesting
