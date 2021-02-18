#include "pch.h"
#include "IndexedTriangleList.h"


namespace At0::VulkanTesting
{
	static constexpr float PI = 3.14159265f;

	IndexedTriangleList::IndexedTriangleList(
		VertexInput input, std::vector<IndexBuffer::Type> indices)
		: m_Vertices(std::move(input)), m_Indices(std::move(indices))
	{
	}

	IndexedTriangleList IndexedTriangleList::UVSphere(VertexLayout layout, int latDiv, int longDiv)
	{
		constexpr float radius = 1.0f;
		const glm::vec4 base = glm::vec4{ 0.0f, 0.0f, radius, 0.0f };
		const float lattitudeAngle = PI / latDiv;
		const float longitudeAngle = 2.0f * PI / longDiv;

		VertexInput vb{ std::move(layout) };
		for (int iLat = 1; iLat < latDiv; iLat++)
		{
			glm::vec4 latBase = base * glm::rotate(glm::mat4(1.0f), lattitudeAngle * iLat,
										   glm::vec3(1.0f, 0.0f, 0.0f));

			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				glm::vec3 calculatedPos =
					latBase * glm::rotate(glm::mat4(1.0f), longitudeAngle * iLong,
								  glm::vec3(0.0f, 0.0f, 1.0f));

				vb.EmplaceBack(calculatedPos);
			}
		}

		// add the cap vertices
		const uint16_t iNorthPole = (uint16_t)vb.Size();
		{
			glm::vec3 northPos = base;
			vb.EmplaceBack(northPos);
		}
		const uint16_t iSouthPole = (uint16_t)vb.Size();
		{
			glm::vec3 southPos = -base;
			vb.EmplaceBack(southPos);
		}

		const auto calcIdx = [latDiv, longDiv](IndexBuffer::Type iLat, IndexBuffer::Type iLong) {
			return iLat * longDiv + iLong;
		};

		std::vector<IndexBuffer::Type> indices;
		for (IndexBuffer::Type iLat = 0; iLat < latDiv - 2; iLat++)
		{
			for (IndexBuffer::Type iLong = 0; iLong < longDiv - 1; iLong++)
			{
				indices.push_back(calcIdx(iLat, iLong));
				indices.push_back(calcIdx(iLat + 1, iLong));
				indices.push_back(calcIdx(iLat, iLong + 1));
				indices.push_back(calcIdx(iLat, iLong + 1));
				indices.push_back(calcIdx(iLat + 1, iLong));
				indices.push_back(calcIdx(iLat + 1, iLong + 1));
			}
			// wrap band
			indices.push_back(calcIdx(iLat, longDiv - 1));
			indices.push_back(calcIdx(iLat + 1, longDiv - 1));
			indices.push_back(calcIdx(iLat, 0));
			indices.push_back(calcIdx(iLat, 0));
			indices.push_back(calcIdx(iLat + 1, longDiv - 1));
			indices.push_back(calcIdx(iLat + 1, 0));
		}

		// cap fans
		for (IndexBuffer::Type iLong = 0; iLong < longDiv - 1; iLong++)
		{
			// north
			indices.push_back(iNorthPole);
			indices.push_back(calcIdx(0, iLong));
			indices.push_back(calcIdx(0, iLong + 1));
			// south
			indices.push_back(calcIdx(latDiv - 2, iLong + 1));
			indices.push_back(calcIdx(latDiv - 2, iLong));
			indices.push_back(iSouthPole);
		}
		// wrap triangles
		// north
		indices.push_back(iNorthPole);
		indices.push_back(calcIdx(0, longDiv - 1));
		indices.push_back(calcIdx(0, 0));
		// south
		indices.push_back(calcIdx(latDiv - 2, 0));
		indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
		indices.push_back(iSouthPole);

		return { std::move(vb), std::move(indices) };
	}
}  // namespace At0::VulkanTesting
