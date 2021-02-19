#include "pch.h"
#include "Mesh.h"

#include "Graphics/Vulkan/GraphicsPipeline.h"
#include "Graphics/Graphics.h"
#include "PointLight.h"


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
		auto& tform = GetEntity().Get<TransformComponent>();

		GetUniforms()["Transforms"]["model"] = tform.GetMatrix();
		GetUniforms()["Transforms"]["view"] = Graphics::Get().SceneCamera.Matrices.View;
		GetUniforms()["Transforms"]["proj"] = Graphics::Get().SceneCamera.Matrices.Perspective;

		TransformComponent& lightTform =
			Graphics::Get().GetPointLight().GetEntity().Get<TransformComponent>();

		GetUniforms()["Lighting"]["lightPos"] = lightTform.Translation;
		GetUniforms()["Lighting"]["viewPos"] = Graphics::Get().SceneCamera.Position;
		GetUniforms()["Lighting"]["lightColor"] = glm::vec3(1.0f, 1.0f, 1.0f);
		GetUniforms()["Lighting"]["objectColor"] = glm::vec3(1.0f, 1.0f, 1.0f);
	}
}  // namespace At0::VulkanTesting
