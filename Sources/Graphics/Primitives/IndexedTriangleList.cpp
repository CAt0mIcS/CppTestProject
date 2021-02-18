#include "pch.h"
#include "IndexedTriangleList.h"

#define _USE_MATH_DEFINES
#include <cmath>


namespace At0::VulkanTesting
{
	static constexpr double PI_D = 3.14159265358979323846;
	static constexpr float PI = 3.14159265f;

	IndexedTriangleList::IndexedTriangleList(
		VertexInput input, std::vector<IndexBuffer::Type> indices)
		: m_Vertices(std::move(input)), m_Indices(std::move(indices))
	{
	}

	IndexedTriangleList IndexedTriangleList::UVSphere(VertexLayout layout)
	{
		VertexInput vertexInput(std::move(layout));
		std::vector<IndexBuffer::Type> indices;

		vertexInput.EmplaceBack(glm::vec3{ 0.0f, 1.0f, 0.0f });
		uint32_t parallels = 11;
		uint32_t meridians = 22;
		for (uint32_t i = 0; i < parallels - 1; ++i)
		{
			const double polar = PI_D * double(i + 1) / double(parallels);
			const double sp = sin(polar);
			const double cp = cos(polar);

			for (uint32_t j = 0; j < meridians; ++j)
			{
				double const azimuth = 2.0 * PI_D * double(j) / double(meridians);
				double const sa = std::sin(azimuth);
				double const ca = std::cos(azimuth);
				double const x = sp * ca;
				double const y = cp;
				double const z = sp * sa;
				vertexInput.EmplaceBack(glm::vec3{ x, y, z });
			}
		}

		for (uint32_t i = 0; i < meridians; ++i)
		{
			uint32_t const a = i + 1;
			uint32_t const b = (i + 1) % meridians + 1;
			indices.emplace_back(0);
			indices.emplace_back(b);
			indices.emplace_back(a);
		}

		for (uint32_t j = 0; j < parallels - 2; ++j)
		{
			uint32_t aStart = j * meridians + 1;
			uint32_t bStart = (j + 1) * meridians + 1;
			for (uint32_t i = 0; i < meridians; ++i)
			{
				const uint32_t a = aStart + i;
				const uint32_t a1 = aStart + (i + 1) % meridians;
				const uint32_t b = bStart + i;
				const uint32_t b1 = bStart + (i + 1) % meridians;

				indices.emplace_back(a);
				indices.emplace_back(a1);
				indices.emplace_back(b1);
				indices.emplace_back(b);
				indices.emplace_back(b1);
				indices.emplace_back(b);
			}
		}

		for (uint32_t i = 0; i < meridians; ++i)
		{
			uint32_t const a = i + meridians * (parallels - 2) + 1;
			uint32_t const b = (i + 1) % meridians + meridians * (parallels - 2) + 1;
			indices.emplace_back((IndexBuffer::Type)(vertexInput.Size() - 1));
			indices.emplace_back(a);
			indices.emplace_back(b);
		}

		return { vertexInput, indices };
	}
}  // namespace At0::VulkanTesting
