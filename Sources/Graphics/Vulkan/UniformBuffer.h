#pragma once

#include "Buffer.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace At0::VulkanTesting
{
	struct UniformBufferObject
	{
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};

	class UniformBuffer : public Buffer
	{
	public:
		UniformBuffer();

		void Update(const UniformBufferObject& ubo);
	};
}  // namespace At0::VulkanTesting
