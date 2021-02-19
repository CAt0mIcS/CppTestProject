#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform Transforms
{
    mat4 modelView;
    mat4 modelViewProj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec3 outViewPos;
layout(location = 1) out vec3 outNormal;


void main()
{
    outViewPos = vec3(vec4(inPosition, 1.0f) * ubo.modelView);
    // outNormal = inNormal * mat3(ubo.modelView);
    outNormal = inNormal;

    gl_Position = ubo.modelViewProj * vec4(inPosition, 1.0);
}