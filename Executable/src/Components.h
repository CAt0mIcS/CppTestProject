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

};

struct RenderComponent
{

};

struct NativeScriptingComponent
{

};