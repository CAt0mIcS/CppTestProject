#include "pch.h"


/**
 * ------------------------ NOTICE ------------------------
 * If the client using the material system wants to change the pipeline we need to know which
 * shaders were in use before (we don't actually, but just pretend we do). But if the user doesn't
 * want to change the pipeline (so the shaders) then we don't need to store the old shaders.
 * This system lets the client specify, which features he/she wants to have enabled.
 *
 * The mesh only cares about the pipeline which is created using all the PipelineConfiguration
 * variables. When the material goes out of Scope, the Mesh still has the pipeline and can continue
 * operating as usual. When the client wants to change something in the material the pipeline will
 * get recreated once he/she calles Mesh::UpdateMaterial
 */

using GraphicsPipeline = uint32_t;
enum class Features
{
	None = 0,
	DynamicPipeline = 1,
	DynamicSpecular = 2
};

/**
 * This will hold all members to describe the pipeline. Each MaterialBase specialization will have a
 * function to fill in the required struct members for the feature. So feature 1 might set member 1
 * to x and feature 2 might set member b to 32. If the same member is set twice, it means the
 * pipeline config is invalid.
 */
struct PipelineConfiguration
{
	std::optional<bool> recreateSwapchain;
	std::optional<bool> recreatePipeline;
};


template<Features flags>
struct MaterialBase
{
};

template<>
struct MaterialBase<Features::DynamicPipeline>
{
public:
	template<typename... Args>
	void Change(Args&&... args)
	{
		std::cout << "Changing shader to: \n";
		shaders = { args... };

		for (std::string sh : shaders)
			std::cout << "\t" << sh << '\n';
	}

	void PopulateConfig(PipelineConfiguration& config)
	{
		assert(!config.recreateSwapchain &&
			   "MaterialBase configuration invalid. RecreateSwapchain already set.");
		config.recreateSwapchain = true;
	}

protected:
	std::vector<std::string> shaders;
};

template<>
struct MaterialBase<Features::DynamicSpecular>
{
public:
	void Change(uint32_t newSpec)
	{
		std::cout << "Changing specular to " << newSpec << '\n';
		specularIntensity = newSpec;
	}

	void PopulateConfig(PipelineConfiguration& config)
	{
		assert(!config.recreatePipeline &&
			   "MaterialBase configuration invalid. RecreatePipeline already set.");
		config.recreatePipeline = true;
	}

protected:
	uint32_t specularIntensity;
};


template<Features flag, typename T, typename = void>
struct HasFlag : std::false_type
{
};

template<Features flag, typename T>
struct HasFlag<flag, T, std::enable_if_t<std::is_base_of_v<MaterialBase<flag>, T>>> : std::true_type
{
};


template<Features... flags>
class Material : MaterialBase<flags>...
{
public:
	Material() {}

	template<Features feature, typename... Args>
	void Set(Args&&... args)
	{
		MaterialBase<feature>::Change(std::forward<Args>(args)...);
	}

	void CreatePipeline()
	{
		PipelineConfiguration config{};
		(MaterialBase<flags>::PopulateConfig(config), ...);

		if (config.recreatePipeline)
			std::cout << "config.recreatePipeline = " << *config.recreatePipeline << '\n';
		if (config.recreateSwapchain)
			std::cout << "config.recreateSwapchain = " << *config.recreateSwapchain << '\n';

		// Resolve pipeline from codex using PipelineConfiguration
		static int i = 0;
		pipeline = MakeRef<GraphicsPipeline>(i);
		++i;
	}

	Ref<GraphicsPipeline> GetPipeline() const { return pipeline; }

private:
	Ref<GraphicsPipeline> pipeline;
};

using DefaultMaterial = Material<>;


class Mesh
{
public:
	/**
	 * The constructor will call the update material function which creates the pipeline in the
	 * material and updates the member stored in the mesh
	 */
	template<Features... flags>
	Mesh(Material<flags...> material = DefaultMaterial{})
	{
		UpdateMaterial(material);
	}

	/**
	 * Once Material::Set has been called after the material is created, the pipeline might need to
	 * change/be recreated. This is handled by this function. When it gets a material it recreates
	 * it and updates the pipeline stored in the mesh
	 */
	template<Features... flags>
	void UpdateMaterial(Material<flags...>& material)
	{
		material.CreatePipeline();
		pipeline = material.GetPipeline();
	}

	void Bind() { std::cout << "Binding pipeline " << *pipeline << '\n'; }

private:
	Ref<GraphicsPipeline> pipeline;
};


int main()
{
	Mesh mesh;

	Material<Features::DynamicPipeline, Features::DynamicSpecular> dynamicMat;
	dynamicMat.Set<Features::DynamicPipeline>("Res/Sh/Def.vert", "Res/Sh/Def.frag");
	dynamicMat.Set<Features::DynamicSpecular>(215);

	mesh.UpdateMaterial(dynamicMat);
	mesh.Bind();

	// After a while
	dynamicMat.Set<Features::DynamicPipeline>("Res/Sh/Deffered.vert", "Res/Sh/Deffered.frag");
	dynamicMat.Set<Features::DynamicSpecular>(33);

	mesh.UpdateMaterial(dynamicMat);
	mesh.Bind();
}
