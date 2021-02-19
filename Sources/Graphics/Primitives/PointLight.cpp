#include "pch.h"
#include "PointLight.h"

#include "Graphics/Vulkan/VertexBuffer.h"
#include "Graphics/Vulkan/IndexBuffer.h"
#include "Graphics/Vulkan/GraphicsPipeline.h"

#include "Vertex.h"
#include "Graphics/Core/Codex.h"
#include "IndexedTriangleList.h"


namespace At0::VulkanTesting
{
	PointLight::PointLight()
	{
		VertexLayout layout;
		layout.Append(VertexLayout::Position3D);

		IndexedTriangleList uvSphereModel = IndexedTriangleList::UVSphere(layout, 0.2f, 32, 64);

		EmplaceBindable(Codex::Resolve<VertexBuffer>(uvSphereModel.GetVertexInput(), "UVSphere"));
		EmplaceBindable(Codex::Resolve<IndexBuffer>(uvSphereModel.GetIndices(), "UVSphere"));

		EmplaceBindable(Codex::Resolve<GraphicsPipeline>(
			layout, std::vector<std::string_view>{ "Resources/Shaders/Default/DefaultShader.vert",
						"Resources/Shaders/Default/DefaultShader.frag" }));
	}
}  // namespace At0::VulkanTesting
