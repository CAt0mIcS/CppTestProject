#pragma once

#include "Bindable.h"
#include "Commands/CommandBuffer.h"

#include <vulkan/vulkan_core.h>


namespace At0::VulkanTesting
{
	class Shader;

	class Pipeline : public Bindable
	{
	public:
		virtual ~Pipeline() = default;

		void Bind(const CommandBuffer& cmdBuff) override
		{
			vkCmdBindPipeline(cmdBuff, GetBindPoint(), *this);
		}

		virtual const VkPipelineLayout& GetLayout() const = 0;
		virtual VkPipelineBindPoint GetBindPoint() const = 0;

		virtual operator const VkPipeline&() const = 0;

		virtual const VkDescriptorSetLayout& GetDescriptorSetLayout() const = 0;
		virtual const VkDescriptorPool& GetDescriptorPool() const = 0;
		virtual const Shader& GetShader() const = 0;
	};
}  // namespace At0::VulkanTesting
