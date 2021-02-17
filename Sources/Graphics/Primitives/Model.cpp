#include "pch.h"
#include "Model.h"

#include "Mesh.h"
#include "Graphics/Core/Codex.h"

#include "Graphics/Graphics.h"
#include "Graphics/Vulkan/IndexBuffer.h"
#include "Graphics/Vulkan/VertexBuffer.h"
#include "Graphics/Vulkan/GraphicsPipeline.h"
#include "Graphics/Vulkan/Renderpass/Renderpass.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <random>


namespace At0::VulkanTesting
{
	Model::Model(std::string_view filename)
	{
		Assimp::Importer imp;
		const auto pScene = imp.ReadFile(
			filename.data(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
								 aiProcess_ConvertToLeftHanded |
								 aiProcess_GenNormals /* | aiProcess_CalcTangentSpace*/);

		if (!pScene)
			RAY_THROW_RUNTIME("Failed to load model: {0}", imp.GetErrorString());

		for (size_t i = 0; i < pScene->mNumMeshes; ++i)
		{
			m_Meshes.emplace_back(ParseMesh(*pScene->mMeshes[i], pScene->mMaterials));
		}

		int nextID = 0;
		// m_RootNode = ParseNode(nextID, *pScene->mRootNode);
	}

	void Model::Translate(glm::vec3 translation)
	{
		for (Scope<Mesh>& mesh : m_Meshes)
		{
			auto& tform = mesh->GetEntity().Get<TransformComponent>();
			tform.Translation += translation;
		}
	}

	void Model::CmdDraw(const CommandBuffer& cmdBuff)
	{
		for (Scope<Mesh>& mesh : m_Meshes)
			mesh->CmdDraw(cmdBuff);
	}

	void Model::Update()
	{
		for (Scope<Mesh>& mesh : m_Meshes)
			mesh->Update();
	}

	Scope<Mesh> Model::ParseMesh(const aiMesh& mesh, const aiMaterial* const* pMaterials)
	{
		std::vector<Ref<Bindable>> bindables;

		using namespace std::string_literals;
		std::string base = "Resources/Models/"s + mesh.mName.C_Str() + "/"s;

		bool hasSpecularMap = false;
		bool hasAlphaGloss = false;
		bool hasNormalMap = false;
		bool hasDiffuseMap = false;
		float shininess = 2.0f;
		glm::vec4 specularColor{ 0.18f, 0.18f, 0.18f, 1.0f };
		glm::vec4 diffuseColor{ 0.45f, 0.45f, 0.85f, 1.0f };

		// if (mesh.mMaterialIndex >= 0)
		//{
		//	auto& material = *pMaterials[mesh.mMaterialIndex];

		//	aiString texFileName;

		//	if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
		//	{
		//		bindables.emplace_back(Codex::Resolve<Texture>(base + texFileName.C_Str()));
		//		hasDiffuseMap = true;
		//	}
		//	else
		//	{
		//		material.Get(AI_MATKEY_COLOR_DIFFUSE, reinterpret_cast<aiColor3D&>(diffuseColor));
		//	}

		//	if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
		//	{
		//		Ref<Texture> tex = Codex::Resolve<Texture>(base + texFileName.C_Str(), 1);
		//		hasAlphaGloss = tex->HasAlpha();
		//		bindables.emplace_back(std::move(tex));
		//		hasSpecularMap = true;
		//	}
		//	else
		//		material.Get(AI_MATKEY_COLOR_SPECULAR, reinterpret_cast<aiColor3D&>(specularColor));

		//	if (!hasAlphaGloss)
		//		material.Get(AI_MATKEY_SHININESS, shininess);

		//	if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
		//	{
		//		Ref<Texture> tex = Codex::Resolve<Texture>(base + texFileName.C_Str(), 2);
		//		hasAlphaGloss = tex->HasAlpha();
		//		bindables.emplace_back(std::move(tex));
		//		hasNormalMap = true;
		//	}

		//	if (hasDiffuseMap || hasSpecularMap || hasNormalMap)
		//	{
		//		bindables.emplace_back(Codex::Resolve<Sampler>());
		//	}
		//}

		static std::mt19937 mtDevice;

		const std::string meshTag = base + "%" + mesh.mName.C_Str();

		VertexLayout layout{};
		layout.Append(VertexLayout::Position3D);
		layout.Append(VertexLayout::Float3Color);
		// layout.Append(VertexLayout::Normal);

		// VK_TODO: Different vertex layouts for different combinations of maps
		if (!hasDiffuseMap && !hasNormalMap && !hasSpecularMap)
		{
			VertexInput vertexInput(layout);

			for (uint32_t i = 0; i < mesh.mNumVertices; ++i)
			{
				std::uniform_real_distribution<float> colDist(0.0f, 1.0f);
				float r = colDist(mtDevice);
				float g = colDist(mtDevice);
				float b = colDist(mtDevice);

				vertexInput.EmplaceBack(
					glm::vec3(mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z),
					glm::vec3(r, g, b)); /*,
glm::vec3(mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z));*/
			}

			std::vector<IndexBuffer::Type> indices;
			indices.reserve(mesh.mNumFaces * 3);

			for (uint32_t i = 0; i < mesh.mNumFaces; ++i)
			{
				const auto& face = mesh.mFaces[i];
				RAY_MEXPECTS(face.mNumIndices == 3, "Model was not triangulated.");
				indices.push_back(face.mIndices[0]);
				indices.push_back(face.mIndices[1]);
				indices.push_back(face.mIndices[2]);
			}

			bindables.emplace_back(Codex::Resolve<VertexBuffer>(std::move(vertexInput), meshTag));
			bindables.emplace_back(Codex::Resolve<IndexBuffer>(std::move(indices), meshTag));

			bindables.emplace_back(Codex::Resolve<GraphicsPipeline>(
				layout, std::vector<std::string_view>{ "Resources/Shaders/PhongModelShader.vert",
							"Resources/Shaders/PhongModelShader.frag" }));
		}

		return MakeScope<Mesh>(std::move(bindables));
	}

	// Scope<Node> Model::ParseNode(int& nextID, const aiNode& node)
	//{
	// const glm::vec4& transform = *reinterpret_cast<const glm::vec4*>(&node.mTransformation);

	// std::vector<Mesh*> curMeshes;
	// curMeshes.resize(node.mNumMeshes);

	// for (size_t i = 0; i < node.mNumMeshes; ++i)
	//{
	//	const auto meshIdx = node.mMeshes[i];
	//	curMeshes.emplace_back(m_Meshes.at(meshIdx).get());
	//}

	// auto pNode = MakeScope<Node>(nextID++, node.mName.C_Str(), std::move(curMeshes),
	// transform); for (size_t i = 0; i < node.mNumChildren; ++i)
	//{
	//	pNode->AddChild(ParseNode(nextID, *node.mChildren[i]));
	//}
	//}
}  // namespace At0::VulkanTesting
