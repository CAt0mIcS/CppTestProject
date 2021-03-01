#include "pch.h"


/**
 * ------------------------ NOTICE ------------------------
 * If the client using the material system wants to change the pipeline we need to know which
 * shaders were in use before (we don't actually, but just pretend we do). But if the use doesn't
 * want to change the pipeline (so the shaders) then we don't need to store the old shaders.
 * This system lets the client specify, which features he/she wants to have enabled.
 */

enum class Features
{
	None = 0,
	DynamicPipeline = 1,
	DynamicSpecular = 2
};


template<Features flags>
struct MaterialBase
{
};

template<>
struct MaterialBase<Features::DynamicPipeline>
{
public:
	void SetShaders(std::vector<std::string> sh) { shaders = std::move(sh); }

protected:
	std::vector<std::string> shaders;
};

template<>
struct MaterialBase<Features::DynamicSpecular>
{
public:
	void SetSpecularIntensity(uint32_t specs) { specularIntensity = specs; }

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
class Material : public MaterialBase<flags>...
{
public:
	Material(float roughness, float shininess) : roughness(roughness), shininess(shininess) {}

	void ChangeShaders(std::vector<std::string> sh)
	{
		if constexpr (HasFlag<Features::DynamicPipeline, Material<flags...>>::value)
		{
			std::cout << "Change shaders here...\n";
		}
		else
			static_assert(false, "Features::DynamicPipeline not present.");
	}

	void ChangeSpecular(uint32_t specs)
	{
		if constexpr (HasFlag<Features::DynamicSpecular, Material<flags...>>::value)
		{
			std::cout << "Changing specular to " << specs << '\n';
		}
		else
			static_assert(false, "Features::DynamicSpecular not present.");
	}

	float GetRoughness() const { return roughness; }
	float GetShininess() const { return shininess; }

	void SetRoughness(float r) { roughness = r; }
	void SetShininess(float s) { shininess = s; }

private:
	float roughness;
	float shininess;
};

using DefaultMaterial = Material<>;


int main()
{
	DefaultMaterial mat(1.0f, 2.0f);
	// mat.ChangeShaders({ "" }); // Fails

	Material<Features::DynamicPipeline, Features::DynamicSpecular> dynamicMat(1.0f, 2.0f);
	dynamicMat.SetShaders({ "Res/Sh/Def.vert", "Res/Sh/Def.frag" });
	dynamicMat.SetSpecularIntensity(1);

	// After a while
	dynamicMat.ChangeShaders({ "Res/Sh/Deffered.vert", "Res/Sh/Deffered.frag" });
	dynamicMat.ChangeSpecular(12);

	// Shouldn't work!
	MaterialBase<Features::DynamicPipeline> dynamicMatCast = dynamicMat;
}
