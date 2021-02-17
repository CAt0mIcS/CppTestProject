#pragma once

#include <string>
#include <memory>

#include <glm/glm.hpp>

struct aiMesh;
struct aiMaterial;
struct aiNode;


namespace At0::VulkanTesting
{
	class Node;
	class Mesh;
	class CommandBuffer;

	class Model
	{
	public:
		Model(std::string_view filename);
		void SetRootTransform(const glm::mat4& transform);

		void CmdDraw(const CommandBuffer& cmdBuff);
		void Update();

	private:
		static Scope<Mesh> ParseMesh(const aiMesh& mesh, const aiMaterial* const* pMaterials);
		Scope<Node> ParseNode(int& nextID, const aiNode& node);

	private:
		// Scope<Node> m_RootNode;
		std::vector<Scope<Mesh>> m_Meshes;
	};
}  // namespace At0::VulkanTesting
