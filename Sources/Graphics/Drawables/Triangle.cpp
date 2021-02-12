#include "Triangle.h"


namespace At0::VulkanTesting
{
	Triangle::Triangle()
		: m_VertexBuffer({ { { 0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
			  { { 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f } }, { { -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } } })
	{
	}

	Triangle::~Triangle() {}
}  // namespace At0::VulkanTesting