#pragma once

#include <vector>
#include <array>

#include "Buffer.h"


namespace At0::VulkanTesting
{
	struct Vertex
	{
		struct
		{
			float x, y, z = 0.0f;
		} pos;

		struct
		{
			float r, g, b;
		} color;


		static VkVertexInputBindingDescription GetBindingDescription(uint32_t binding = 0)
		{
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = binding;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions(
			uint32_t posLocation, uint32_t colorLocation, uint32_t binding = 0)
		{
			std::array<VkVertexInputAttributeDescription, 2> attributeDescs{};

			attributeDescs[0].binding = binding;
			attributeDescs[0].location = posLocation;
			attributeDescs[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescs[0].offset = offsetof(Vertex, pos);

			attributeDescs[1].binding = binding;
			attributeDescs[1].location = colorLocation;
			attributeDescs[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescs[1].offset = offsetof(Vertex, color);

			return attributeDescs;
		}
	};

	class VertexBuffer : public Buffer
	{
	public:
		VertexBuffer(const std::vector<Vertex> vertices);
		~VertexBuffer() = default;
	};
}  // namespace At0::VulkanTesting
