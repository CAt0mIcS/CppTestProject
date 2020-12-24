#pragma once

#include <string>


struct TagComponent
{
	std::string tag;

	TagComponent(const std::string_view tag) : tag(tag) {}
	TagComponent() = default;
};

struct TransformComponent
{
	float x;

	TransformComponent(float x) : x(x) {}
	TransformComponent() = default;
};

struct SpriteRenderComponent
{
	uint32_t SpriteRendererID;

	SpriteRenderComponent(uint32_t x) : SpriteRendererID(x) {}
	SpriteRenderComponent() = default;
};

struct RenderComponent
{
	uint32_t RendererID;

	RenderComponent(uint32_t x) : RendererID(x) {}
	RenderComponent() = default;
};

struct NativeScriptingComponent
{
	std::string ScriptName;

	NativeScriptingComponent(const std::string_view name) : ScriptName(name) {}
	NativeScriptingComponent() = default;
};