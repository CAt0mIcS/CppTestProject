#include "pch.h"
#include "Mesh.h"

#include "Graphics/Vulkan/GraphicsPipeline.h"


namespace At0::VulkanTesting
{
	Mesh::Mesh(std::vector<Ref<Bindable>> bindables)
	{
		for (auto& bindable : bindables)
		{
			EmplaceBindable(bindable);
		}
	}

	void Mesh::CmdDraw(const CommandBuffer& cmdBuff) { Drawable::CmdDraw(cmdBuff); }
	void Mesh::Update()
	{
		Drawable::Update();
		const glm::vec3 lightColor{ 1.0f, 0.0f, 0.0f };
		const glm::vec3 lightPos{ 0.0f, 0.0f, 0.0f };

		// GetUniforms()["Lighting"]["lightColor"] = lightColor;
		GetUniforms()["Lighting"]["lightPos"] = lightPos;
	}
}  // namespace At0::VulkanTesting
