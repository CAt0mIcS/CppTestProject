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

	class UniformBufferView
	{
		friend class UniformHandler;

	public:
		template<typename T>
		UniformBufferView& operator=(const T& data)
		{
			m_UniformBuffer.Update(&data);
			return *this;
		}

	private:
		UniformBufferView(UniformBuffer& ubuff) : m_UniformBuffer(ubuff) {}

	private:
		UniformBuffer& m_UniformBuffer;
	};

	class UniformHandler
	{
	public:
		UniformHandler(const Pipeline& pipeline);
		~UniformHandler();

		void BindDescriptors(const CommandBuffer& cmdBuff, const Pipeline& pipeline);
		void UpdateDescriptors();

		UniformBufferView operator[](std::string_view uniformName);

	private:
		std::unordered_map<std::string, UniformBuffer*> m_UniformBuffers;
		std::vector<DescriptorSet*> m_DescriptorSets;
	};
}  // namespace At0::VulkanTesting
