#pragma once

#include <string>


struct TagComponent
{
	std::string tag;

	TagComponent(const std::string_view tag) : tag(tag) {}
};

struct TransformComponent
{

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