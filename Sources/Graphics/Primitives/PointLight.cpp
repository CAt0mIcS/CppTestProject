﻿#include "pch.h"
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

		IndexedTriangleList uvSphereModel = IndexedTriangleList::UVSphere(layout);

		EmplaceBindable(Codex::Resolve<VertexBuffer>(uvSphereModel.GetVertexInput(), "UVSphere"));
		EmplaceBindable(Codex::Resolve<IndexBuffer>(uvSphereModel.GetIndices(), "UVSphere"));
	}

	void PointLight::CmdDraw(const CommandBuffer& cmdBuff)
	{
		m_DescriptorsHandler.Push("UniformBufferObject", m_UniformHandler);
		m_DescriptorsHandler.Update(GetGraphicsPipeline());
		m_DescriptorsHandler.BindDescriptor(cmdBuff, GetGraphicsPipeline());

		Drawable::CmdDraw(cmdBuff);
	}
}  // namespace At0::VulkanTesting
