#pragma once

#include <string>
#include <map>
#include <optional>

#include "Graphics/Vulkan/Descriptor.h"
#include "Graphics/Vulkan/UniformBuffer.h"


namespace At0::VulkanTesting
{
	class Pipeline;
	class UniformHandler;
	class Shader;

	class DescriptorsHandler
	{
	public:
		DescriptorsHandler() = default;
		DescriptorsHandler(const Pipeline& pipeline);

		template<typename T>
		void Push(std::string_view descriptorName, const T& descriptor)
		{
			if (!m_Shader)
				return;

			// Finds the local value given to the descriptor name.
			auto it = m_Descriptors.find(descriptorName.data());

			if (it != m_Descriptors.end())
			{
				// If the descriptor and size have not changed then the write is not modified.
				if (it->second.descriptor == &descriptor)
				{
					return;
				}

				m_Descriptors.erase(it);
			}

			// When adding the descriptor find the location in the shader.
			auto location = m_Shader->GetDescriptorLocation(descriptorName);

			if (!location)
			{
#ifndef NDEBUG
				if (m_Shader->ReportedNotFound(descriptorName, true))
				{
					Log::Error("Could not find descriptor in m_Shader");
				}
#endif

				return;
			}

			auto descriptorType = m_Shader->GetDescriptorType(*location);

			if (!descriptorType)
			{
#ifndef NDEBUG
				if (m_Shader->ReportedNotFound(descriptorName, true))
				{
					Log::Error("Could not find descriptor in m_Shader");
				}
#endif
				return;
			}

			// Adds the new descriptor value.
			auto writeDescriptor = descriptor.GetWriteDescriptor(*location, *descriptorType);
			m_Descriptors.emplace(descriptorName,
				DescriptorValue{ &descriptor, std::move(writeDescriptor), *location });
			m_Changed = true;
		}

		template<typename T>
		void Push(std::string_view descriptorName, const T& descriptor,
			WriteDescriptorSet writeDescriptorSet)
		{
			if (!m_Shader)
				return;

			if (auto it = m_Descriptors.find(descriptorName.data()); it != m_Descriptors.end())
			{
				m_Descriptors.erase(it);
			}

			auto location = m_Shader->GetDescriptorLocation(descriptorName);
			// auto descriptorType = shader->GetDescriptorType(*location);

			m_Descriptors.emplace(descriptorName,
				DescriptorValue{ &descriptor, std::move(writeDescriptorSet), *location });
			m_Changed = true;
		}

		void Push(const std::string& descriptorName, UniformHandler& uniformHandler);
		bool Update(const Pipeline& pipeline);
		void BindDescriptor(const CommandBuffer& commandBuffer, const Pipeline& pipeline);

		const DescriptorSet& GetDescriptorSet() const { return *m_DescriptorSet; }

	private:
		class DescriptorValue
		{
		public:
			const Descriptor* descriptor;
			WriteDescriptorSet writeDescriptor;
			uint32_t location;
		};

		const Shader* m_Shader = nullptr;
		bool m_PushDescriptors = false;
		Scope<DescriptorSet> m_DescriptorSet;

		std::map<std::string, DescriptorValue> m_Descriptors;
		std::vector<VkWriteDescriptorSet> m_WriteDescriptorSets;
		bool m_Changed = false;
	};
}  // namespace At0::VulkanTesting
