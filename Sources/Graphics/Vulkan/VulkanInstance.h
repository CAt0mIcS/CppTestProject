#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>

#include "Base.h"


namespace At0::VulkanTesting
{
	class VulkanInstance
	{
	public:
		VulkanInstance();
		~VulkanInstance();

		operator const VkInstance&() const { return m_Instance; }

		bool ValidationLayersEnabled() const { return m_ValidationLayerEnabled; }
		static const std::vector<const char*>& ValidationLayers() { return s_ValidationLayers; }

		static void FvkCmdPushDescriptorSetKHR(VkDevice device, VkCommandBuffer commandBuffer,
			VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set,
			uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites);

	private:
		void CreateInstance();
		void CreateDebugMessenger();
		bool CheckValidationLayerSupport() const;
		std::vector<const char*> GetExtensions() const;

	private:
		VkInstance m_Instance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;

#ifndef NDEBUG
		bool m_ValidationLayerEnabled = true;
		inline static const std::vector<const char*> s_ValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
#else
		bool m_ValidationLayerEnabled = false;
		inline static const std::vector<const char*> s_ValidationLayers = {};
#endif
	};
}  // namespace At0::VulkanTesting
