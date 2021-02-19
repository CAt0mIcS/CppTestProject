#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 modelView;
    mat4 modelViewProj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec3 outWorldPos;
layout(location = 1) out vec3 outNormal;


void main()
{
    outWorldPos = (vec4(inPosition, 1.0) * ubo.modelView).xyz;
    outNormal = inNormal * mat3(ubo.modelView);
    gl_Position = ubo.modelViewProj * vec4(inPosition, 1.0);
}