#include "pch.h"
#include "Shader.h"
#include "Graphics/Graphics.h"
#include "Utils/RString.h"

#include <glslang/SPIRV/GlslangToSpv.h>


namespace At0::VulkanTesting
{
	class SHaderIncluder : public glslang::TShader::Includer
	{
	public:
		IncludeResult* includeLocal(
			const char* headerName, const char* includerName, size_t inclusionDepth) override
		{
			std::filesystem::path directory = std::filesystem::path(includerName).parent_path();
			std::optional<std::string> fileLoaded = Shader::ReadFile(directory / headerName);

			if (!fileLoaded)
			{
				Log::Error("Shader include could not be loaded: {0}", headerName);
				return nullptr;
			}

			char* content = new char[fileLoaded->size()];
			memcpy(content, fileLoaded->c_str(), fileLoaded->size());
			return new IncludeResult(headerName, content, fileLoaded->size(), content);
		}

		IncludeResult* includeSystem(
			const char* headerName, const char* includerName, size_t inclusionDepth) override
		{
			std::optional<std::string> fileLoaded = Shader::ReadFile(headerName);

			if (!fileLoaded)
			{
				Log::Error("Shader include could not be loaded: {0}", headerName);
				return nullptr;
			}

			char* content = new char[fileLoaded->size()];
			memcpy(content, fileLoaded->c_str(), fileLoaded->size());
			return new IncludeResult(headerName, content, fileLoaded->size(), content);
		}

		void releaseInclude(IncludeResult* result) override
		{
			if (result)
			{
				delete[]((char*)result->userData);
				delete result;
			}
		}
	};


	Shader::Shader() {}
	bool Shader::ReportedNotFound(std::string_view name, bool reportIfFound) const
	{
		if (std::find(m_NotFoundNames.begin(), m_NotFoundNames.end(), name) ==
			m_NotFoundNames.end())
		{
			if (reportIfFound)
				m_NotFoundNames.emplace_back(name);

			return true;
		}

		return false;
	}

	VkFormat Shader::GlTypeToVk(int32_t type)
	{
		switch (type)
		{
		case 0x1406:  // GL_FLOAT
			return VK_FORMAT_R32_SFLOAT;
		case 0x8B50:  // GL_FLOAT_VEC2
			return VK_FORMAT_R32G32_SFLOAT;
		case 0x8B51:  // GL_FLOAT_VEC3
			return VK_FORMAT_R32G32B32_SFLOAT;
		case 0x8B52:  // GL_FLOAT_VEC4
			return VK_FORMAT_R32G32B32A32_SFLOAT;
		case 0x1404:  // GL_INT
			return VK_FORMAT_R32_SINT;
		case 0x8B53:  // GL_INT_VEC2
			return VK_FORMAT_R32G32_SINT;
		case 0x8B54:  // GL_INT_VEC3
			return VK_FORMAT_R32G32B32_SINT;
		case 0x8B55:  // GL_INT_VEC4
			return VK_FORMAT_R32G32B32A32_SINT;
		case 0x1405:  // GL_UNSIGNED_INT
			return VK_FORMAT_R32_SINT;
		case 0x8DC6:  // GL_UNSIGNED_INT_VEC2
			return VK_FORMAT_R32G32_SINT;
		case 0x8DC7:  // GL_UNSIGNED_INT_VEC3
			return VK_FORMAT_R32G32B32_SINT;
		case 0x8DC8:  // GL_UNSIGNED_INT_VEC4
			return VK_FORMAT_R32G32B32A32_SINT;
		default: return VK_FORMAT_UNDEFINED;
		}
	}

	std::optional<uint32_t> Shader::GetDescriptorLocation(std::string_view name) const
	{
		if (auto it = m_DescriptorLocations.find(name.data()); it != m_DescriptorLocations.end())
			return it->second;
		return std::nullopt;
	}

	std::optional<uint32_t> Shader::GetDescriptorSize(std::string_view name) const
	{
		if (auto it = m_DescriptorSizes.find(name.data()); it != m_DescriptorSizes.end())
			return it->second;
		return std::nullopt;
	}

	std::optional<Shader::Uniform> Shader::GetUniform(std::string_view name) const
	{
		if (auto it = m_Uniforms.find(name.data()); it != m_Uniforms.end())
			return it->second;
		return std::nullopt;
	}

	std::optional<Shader::UniformBlock> Shader::GetUniformBlock(std::string_view name) const
	{
		if (auto it = m_UniformBlocks.find(name.data()); it != m_UniformBlocks.end())
			return it->second;
		return std::nullopt;
	}

	std::optional<Shader::Attribute> Shader::GetAttribute(std::string_view name) const
	{
		if (auto it = m_Attributes.find(name.data()); it != m_Attributes.end())
			return it->second;
		return std::nullopt;
	}

	std::vector<VkPushConstantRange> Shader::GetPushConstantRanges() const
	{
		std::vector<VkPushConstantRange> pushConstantRanges;
		uint32_t currentOffset = 0;

		for (const auto& [uniformBlockName, uniformBlock] : m_UniformBlocks)
		{
			if (uniformBlock.GetType() != UniformBlock::Type::Push)
				continue;

			VkPushConstantRange pushConstantRange{};
			pushConstantRange.stageFlags = uniformBlock.GetStageFlags();
			pushConstantRange.offset = currentOffset;
			pushConstantRange.size = (uint32_t)uniformBlock.GetSize();
			pushConstantRanges.emplace_back(pushConstantRange);
			currentOffset += pushConstantRange.size;
		}

		return pushConstantRanges;
	}

	std::optional<VkDescriptorType> Shader::GetDescriptorType(uint32_t location) const
	{
		if (auto it = m_DescriptorTypes.find(location); it != m_DescriptorTypes.end())
			return it->second;
		return std::nullopt;
	}

	VkShaderStageFlagBits Shader::GetShaderStage(const std::filesystem::path& filepath)
	{
		std::string fileExt = filepath.extension().string();
		std::transform(fileExt.begin(), fileExt.end(), fileExt.begin(), ::tolower);

		if (fileExt == ".comp")
			return VK_SHADER_STAGE_COMPUTE_BIT;
		if (fileExt == ".vert")
			return VK_SHADER_STAGE_VERTEX_BIT;
		if (fileExt == ".tesc")
			return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		if (fileExt == ".tese")
			return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		if (fileExt == ".geom")
			return VK_SHADER_STAGE_GEOMETRY_BIT;
		if (fileExt == ".frag")
			return VK_SHADER_STAGE_FRAGMENT_BIT;
		return VK_SHADER_STAGE_ALL;
	}

	EShLanguage GetEshLanguage(VkShaderStageFlags stageFlag)
	{
		switch (stageFlag)
		{
		case VK_SHADER_STAGE_COMPUTE_BIT: return EShLangCompute;
		case VK_SHADER_STAGE_VERTEX_BIT: return EShLangVertex;
		case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT: return EShLangTessControl;
		case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT: return EShLangTessEvaluation;
		case VK_SHADER_STAGE_GEOMETRY_BIT: return EShLangGeometry;
		case VK_SHADER_STAGE_FRAGMENT_BIT: return EShLangFragment;
		default: return EShLangCount;
		}
	}

	TBuiltInResource GetResources()
	{
		TBuiltInResource resources = {};
		resources.maxLights = 32;
		resources.maxClipPlanes = 6;
		resources.maxTextureUnits = 32;
		resources.maxTextureCoords = 32;
		resources.maxVertexAttribs = 64;
		resources.maxVertexUniformComponents = 4096;
		resources.maxVaryingFloats = 64;
		resources.maxVertexTextureImageUnits = 32;
		resources.maxCombinedTextureImageUnits = 80;
		resources.maxTextureImageUnits = 32;
		resources.maxFragmentUniformComponents = 4096;
		resources.maxDrawBuffers = 32;
		resources.maxVertexUniformVectors = 128;
		resources.maxVaryingVectors = 8;
		resources.maxFragmentUniformVectors = 16;
		resources.maxVertexOutputVectors = 16;
		resources.maxFragmentInputVectors = 15;
		resources.minProgramTexelOffset = -8;
		resources.maxProgramTexelOffset = 7;
		resources.maxClipDistances = 8;
		resources.maxComputeWorkGroupCountX = 65535;
		resources.maxComputeWorkGroupCountY = 65535;
		resources.maxComputeWorkGroupCountZ = 65535;
		resources.maxComputeWorkGroupSizeX = 1024;
		resources.maxComputeWorkGroupSizeY = 1024;
		resources.maxComputeWorkGroupSizeZ = 64;
		resources.maxComputeUniformComponents = 1024;
		resources.maxComputeTextureImageUnits = 16;
		resources.maxComputeImageUniforms = 8;
		resources.maxComputeAtomicCounters = 8;
		resources.maxComputeAtomicCounterBuffers = 1;
		resources.maxVaryingComponents = 60;
		resources.maxVertexOutputComponents = 64;
		resources.maxGeometryInputComponents = 64;
		resources.maxGeometryOutputComponents = 128;
		resources.maxFragmentInputComponents = 128;
		resources.maxImageUnits = 8;
		resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
		resources.maxCombinedShaderOutputResources = 8;
		resources.maxImageSamples = 0;
		resources.maxVertexImageUniforms = 0;
		resources.maxTessControlImageUniforms = 0;
		resources.maxTessEvaluationImageUniforms = 0;
		resources.maxGeometryImageUniforms = 0;
		resources.maxFragmentImageUniforms = 8;
		resources.maxCombinedImageUniforms = 8;
		resources.maxGeometryTextureImageUnits = 16;
		resources.maxGeometryOutputVertices = 256;
		resources.maxGeometryTotalOutputComponents = 1024;
		resources.maxGeometryUniformComponents = 1024;
		resources.maxGeometryVaryingComponents = 64;
		resources.maxTessControlInputComponents = 128;
		resources.maxTessControlOutputComponents = 128;
		resources.maxTessControlTextureImageUnits = 16;
		resources.maxTessControlUniformComponents = 1024;
		resources.maxTessControlTotalOutputComponents = 4096;
		resources.maxTessEvaluationInputComponents = 128;
		resources.maxTessEvaluationOutputComponents = 128;
		resources.maxTessEvaluationTextureImageUnits = 16;
		resources.maxTessEvaluationUniformComponents = 1024;
		resources.maxTessPatchComponents = 120;
		resources.maxPatchVertices = 32;
		resources.maxTessGenLevel = 64;
		resources.maxViewports = 16;
		resources.maxVertexAtomicCounters = 0;
		resources.maxTessControlAtomicCounters = 0;
		resources.maxTessEvaluationAtomicCounters = 0;
		resources.maxGeometryAtomicCounters = 0;
		resources.maxFragmentAtomicCounters = 8;
		resources.maxCombinedAtomicCounters = 8;
		resources.maxAtomicCounterBindings = 1;
		resources.maxVertexAtomicCounterBuffers = 0;
		resources.maxTessControlAtomicCounterBuffers = 0;
		resources.maxTessEvaluationAtomicCounterBuffers = 0;
		resources.maxGeometryAtomicCounterBuffers = 0;
		resources.maxFragmentAtomicCounterBuffers = 1;
		resources.maxCombinedAtomicCounterBuffers = 1;
		resources.maxAtomicCounterBufferSize = 16384;
		resources.maxTransformFeedbackBuffers = 4;
		resources.maxTransformFeedbackInterleavedComponents = 64;
		resources.maxCullDistances = 8;
		resources.maxCombinedClipAndCullDistances = 8;
		resources.maxSamples = 4;
		resources.limits.nonInductiveForLoops = true;
		resources.limits.whileLoops = true;
		resources.limits.doWhileLoops = true;
		resources.limits.generalUniformIndexing = true;
		resources.limits.generalAttributeMatrixVectorIndexing = true;
		resources.limits.generalVaryingIndexing = true;
		resources.limits.generalSamplerIndexing = true;
		resources.limits.generalVariableIndexing = true;
		resources.limits.generalConstantMatrixVectorIndexing = true;
		return resources;
	}

	VkShaderModule Shader::CreateShaderModule(const std::filesystem::path& moduleName,
		std::string_view moduleCode, std::string_view preamble, VkShaderStageFlags moduleFlag)
	{
		m_Stages.emplace_back(moduleName.string());

		// Start converting GLSL to SPIR-V
		EShLanguage language = GetEshLanguage(moduleFlag);
		glslang::TProgram program;
		glslang::TShader shader(language);
		TBuiltInResource resources = GetResources();

		// Enable SPIR-V ande Vulkan rules when paarsing GLSL
		EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules | EShMsgDefault);
#ifndef NDEBUG
		messages = (EShMessages)(messages | EShMsgDebugInfo);
#endif

		std::string shaderName = moduleName.string();
		const char* shaderNameCstr = shaderName.c_str();
		const char* shaderSource = moduleCode.data();

		shader.setStringsWithLengthsAndNames(&shaderSource, nullptr, &shaderNameCstr, 1);
		shader.setPreamble(preamble.data());

		glslang::EShTargetClientVersion defaultVersion = glslang::EShTargetVulkan_1_1;
		shader.setEnvInput(glslang::EShSourceGlsl, language, glslang::EShClientVulkan, 110);
		shader.setEnvClient(glslang::EShClientVulkan, defaultVersion);
		shader.setEnvTarget(
			glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);	// VK_TODO: Support higher versions

		SHaderIncluder includer;

		std::string str;

		if (!shader.preprocess(
				&resources, defaultVersion, ENoProfile, false, false, messages, &str, includer))
		{
			Log::Error("[Shader] {0}", shader.getInfoLog());
			Log::Error("[Shader] {0}", shader.getInfoDebugLog());
			Log::Error("SPIR-V m_Shader preprocess failed!");
		}

		if (!shader.parse(&resources, defaultVersion, true, messages, includer))
		{
			Log::Error("[Shader] {0}", shader.getInfoLog());
			Log::Error("[Shader] {0}", shader.getInfoDebugLog());
			Log::Error("SPIR-V m_Shader parse failed!");
		}

		program.addShader(&shader);

		if (!program.link(messages) || !program.mapIO())
		{
			Log::Error("Error while linking m_Shader program!");
		}

		program.buildReflection();

		for (uint32_t dim = 0; dim < 3; ++dim)
		{
			if (auto localSize = program.getLocalSize(dim); localSize > 1)
				m_LocalSizes[dim] = localSize;
		}

		for (int32_t i = program.getNumLiveUniformBlocks() - 1; i >= 0; i--)
			LoadUniformBlock(program, moduleFlag, i);

		for (int32_t i = 0; i < program.getNumLiveUniformVariables(); i++)
			LoadUniform(program, moduleFlag, i);

		for (int32_t i = 0; i < program.getNumLiveAttributes(); i++)
			LoadAttribute(program, moduleFlag, i);

		glslang::SpvOptions spvOptions;
#ifndef NDEBUG
		spvOptions.generateDebugInfo = true;
		spvOptions.disableOptimizer = true;
		spvOptions.optimizeSize = false;
#else
		spvOptions.generateDebugInfo = false;
		spvOptions.disableOptimizer = false;
		spvOptions.optimizeSize = true;
#endif

		spv::SpvBuildLogger logger;
		std::vector<uint32_t> spirv;
		GlslangToSpv(*program.getIntermediate((EShLanguage)language), spirv, &logger, &spvOptions);

		VkShaderModuleCreateInfo shaderModuleCreateInfo{};
		shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderModuleCreateInfo.codeSize = spirv.size() * sizeof(uint32_t);
		shaderModuleCreateInfo.pCode = spirv.data();

		VkShaderModule shaderModule;
		RAY_VK_THROW_FAILED(vkCreateShaderModule(Graphics::Get().GetLogicalDevice(),
								&shaderModuleCreateInfo, nullptr, &shaderModule),
			"Failed to create shade module");

		return shaderModule;
	}

	void Shader::CreateReflection()
	{
		std::map<VkDescriptorType, uint32_t> descriptorPoolCounts;

		// Process to descriptors
		for (const auto& [uniformBlockName, uniformBlock] : m_UniformBlocks)
		{
			VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_MAX_ENUM;

			switch (uniformBlock.GetType())
			{
			case UniformBlock::Type::Uniform:
				descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				m_DescriptorSetLayouts.emplace_back(
					UniformBuffer::GetDescriptorSetLayout((uint32_t)uniformBlock.GetBinding(),
						descriptorType, uniformBlock.GetStageFlags(), 1));
				break;
			// case UniformBlock::Type::Storage:
			//	descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			//	m_DescriptorSetLayouts.emplace_back(
			//		StorageBuffer::GetDescriptorSetLayout((uint32_t)uniformBlock.GetBinding(),
			//			descriptorType, uniformBlock.GetStageFlags(), 1));
			//	break;
			case UniformBlock::Type::Push:
				// Push constants are described in the pipeline.
				break;
			default: break;
			}

			IncrementDescriptorPool(descriptorPoolCounts, descriptorType);
			m_DescriptorLocations.emplace(uniformBlockName, uniformBlock.GetBinding());
			m_DescriptorSizes.emplace(uniformBlockName, uniformBlock.GetSize());
		}

		for (const auto& [uniformName, uniform] : m_Uniforms)
		{
			VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_MAX_ENUM;
			// switch (uniform.GetGlType())
			//{
			// case 0x8B5E:  // GL_SAMPLER_2D
			// case 0x904D:  // GL_IMAGE_2D
			// case 0x8DC1:  // GL_TEXTURE_2D_ARRAY
			// case 0x9108:  // GL_SAMPLER_2D_MULTISAMPLE
			// case 0x9055:  // GL_IMAGE_2D_MULTISAMPLE
			//	descriptorType = uniform.IsWriteOnly() ? VK_DESCRIPTOR_TYPE_STORAGE_IMAGE :
			//											 VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			//	m_DescriptorSetLayouts.emplace_back(Image2D::GetDescriptorSetLayout(
			//		(uint32_t)uniform.GetBinding(), descriptorType, uniform.GetStageFlags(), 1));
			//	break;
			// case 0x8B60:  // GL_SAMPLER_CUBE
			// case 0x9050:  // GL_IMAGE_CUBE
			// case 0x9054:  // GL_IMAGE_CUBE_MAP_ARRAY
			//	descriptorType = uniform.IsWriteOnly() ? VK_DESCRIPTOR_TYPE_STORAGE_IMAGE :
			//											 VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			//	m_DescriptorSetLayouts.emplace_back(ImageCube::GetDescriptorSetLayout(
			//		(uint32_t)uniform.GetBinding(), descriptorType, uniform.GetStageFlags(), 1));
			//	break;
			// default: break;
			//}

			IncrementDescriptorPool(descriptorPoolCounts, descriptorType);
			m_DescriptorLocations.emplace(uniformName, uniform.GetBinding());
			m_DescriptorSizes.emplace(uniformName, uniform.GetSize());
		}

		for (const auto& [type, descriptorCount] : descriptorPoolCounts)
		{
			VkDescriptorPoolSize poolSize{};
			poolSize.type = type;
			poolSize.descriptorCount = descriptorCount;
			m_DescriptorPools.emplace_back(poolSize);
		}

		// VK_TODO: This is a AMD workaround that works on NVidia too...
		// We don't know the total usages of descriptor types by the pipeline.
		m_DescriptorPools.resize(6);
		m_DescriptorPools[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		m_DescriptorPools[0].descriptorCount = 4096;
		m_DescriptorPools[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		m_DescriptorPools[1].descriptorCount = 2048;
		m_DescriptorPools[2].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		m_DescriptorPools[2].descriptorCount = 2048;
		m_DescriptorPools[3].type = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
		m_DescriptorPools[3].descriptorCount = 2048;
		m_DescriptorPools[4].type = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
		m_DescriptorPools[4].descriptorCount = 2048;
		m_DescriptorPools[5].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		m_DescriptorPools[5].descriptorCount = 2048;

		// Sort descriptors by binding.
		std::sort(m_DescriptorSetLayouts.begin(), m_DescriptorSetLayouts.end(),
			[](const VkDescriptorSetLayoutBinding& l, const VkDescriptorSetLayoutBinding& r) {
				return l.binding < r.binding;
			});

		// Gets the last descriptors binding.
		if (!m_DescriptorSetLayouts.empty())
			m_LastDescriptorBinding = m_DescriptorSetLayouts.back().binding;

		// Gets the descriptor type for each descriptor.
		for (const auto& descriptor : m_DescriptorSetLayouts)
			m_DescriptorTypes.emplace(descriptor.binding, descriptor.descriptorType);

		// Process attribute descriptions.
		uint32_t currentOffset = 4;

		for (const auto& [attributeName, attribute] : m_Attributes)
		{
			VkVertexInputAttributeDescription attributeDescription = {};
			attributeDescription.location = static_cast<uint32_t>(attribute.GetLocation());
			attributeDescription.binding = 0;
			attributeDescription.format = GlTypeToVk(attribute.GetGlType());
			attributeDescription.offset = currentOffset;
			m_AttributeDescriptions.emplace_back(attributeDescription);
			currentOffset += attribute.GetSize();
		}
	}

	std::optional<std::string> Shader::ReadFile(const std::filesystem::path& path)
	{
		std::ifstream reader(path, std::ios::ate);

		if (!reader.is_open())
			return std::nullopt;

		size_t fileSize = (size_t)reader.tellg();
		std::string data(fileSize, '\0');
		reader.seekg(std::ios::beg);
		reader.read(data.data(), fileSize);

		return data;
	}

	void Shader::IncrementDescriptorPool(
		std::map<VkDescriptorType, uint32_t>& descriptorPoolCounts, VkDescriptorType type)
	{
		if (type == VK_DESCRIPTOR_TYPE_MAX_ENUM)
			return;

		if (auto it = descriptorPoolCounts.find(type); it != descriptorPoolCounts.end())
			it->second++;
		else
			descriptorPoolCounts.emplace(type, 1);
	}

	void Shader::LoadUniformBlock(
		const glslang::TProgram& program, VkShaderStageFlags stageFlag, int32_t i)
	{
		auto reflection = program.getUniformBlock(i);

		for (auto& [uniformBlockName, uniformBlock] : m_UniformBlocks)
		{
			if (uniformBlockName == reflection.name)
			{
				uniformBlock.m_StageFlags |= stageFlag;
				return;
			}
		}

		UniformBlock::Type type = UniformBlock::Type::None;
		if (reflection.getType()->getQualifier().storage == glslang::EvqUniform)
			type = UniformBlock::Type::Uniform;
		if (reflection.getType()->getQualifier().storage == glslang::EvqBuffer)
			type = UniformBlock::Type::Storage;
		if (reflection.getType()->getQualifier().layoutPushConstant)
			type = UniformBlock::Type::Push;

		m_UniformBlocks.emplace(reflection.name,
			UniformBlock(reflection.getBinding(), reflection.size, stageFlag, type));
	}

	void Shader::LoadUniform(
		const glslang::TProgram& program, VkShaderStageFlags stageFlag, int32_t i)
	{
		auto reflection = program.getUniform(i);

		if (reflection.getBinding() == -1)
		{
			std::vector<std::string> splitName = String::Split(reflection.name, '.');

			if (splitName.size() > 0)
			{
				for (auto& [uniformBlockname, uniformBlock] : m_UniformBlocks)
				{
					if (uniformBlockname == splitName.at(0))
					{
						uniformBlock.m_Uniforms.emplace(
							String::ReplaceFirst(reflection.name, splitName.at(0) + ".", ""),
							Uniform(reflection.getBinding(), reflection.offset,
								ComputeSize(reflection.getType()), reflection.glDefineType, false,
								false, stageFlag));
						return;
					}
				}
			}
		}

		for (auto& [uniformName, uniform] : m_Uniforms)
		{
			if (uniformName == reflection.name)
			{
				uniform.m_StageFlags |= stageFlag;
				return;
			}
		}

		auto& qualifier = reflection.getType()->getQualifier();
		m_Uniforms.emplace(reflection.name,
			Uniform(reflection.getBinding(), reflection.offset, -1, reflection.glDefineType,
				qualifier.readonly, qualifier.writeonly, stageFlag));
	}

	void Shader::LoadAttribute(
		const glslang::TProgram& program, VkShaderStageFlags stageFlags, int32_t i)
	{
		auto reflection = program.getPipeInput(i);

		if (reflection.name.empty())
			return;

		for (const auto& [attributeName, attribute] : m_Attributes)
		{
			if (attributeName == reflection.name)
				return;
		}

		auto& qualifier = reflection.getType()->getQualifier();
		m_Attributes.emplace(
			reflection.name, Attribute(qualifier.layoutSet, qualifier.layoutLocation,
								 ComputeSize(reflection.getType()), reflection.glDefineType));
	}

	int32_t Shader::ComputeSize(const glslang::TType* ttype)
	{
		// VK_TODO: glslang::TType::computeNumComponents is available but has many issues resolved
		// in this method.

		int32_t components = 0;

		if (ttype->getBasicType() == glslang::EbtStruct ||
			ttype->getBasicType() == glslang::EbtBlock)
		{
			for (const auto& tl : *ttype->getStruct())
				components += ComputeSize(tl.type);
		}
		else if (ttype->getMatrixCols() != 0)
			components = ttype->getMatrixCols() * ttype->getMatrixRows();
		else
			components = ttype->getVectorSize();

		if (ttype->getArraySizes())
		{
			int32_t arraySize = 1;

			for (int32_t d = 0; d < ttype->getArraySizes()->getNumDims(); ++d)
			{
				// This only makes sense in paths that have a known array size.
				if (auto dimSize = ttype->getArraySizes()->getDimSize(d);
					dimSize != glslang::UnsizedArraySize)
					arraySize *= dimSize;
			}

			components *= arraySize;
		}

		return sizeof(float) * components;
	}
}  // namespace At0::VulkanTesting
