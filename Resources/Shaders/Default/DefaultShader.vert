#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform Transforms
{
    mat4 modelView;
    mat4 modelViewProj;
} ubo;


layout(location = 0) in vec3 inPosition;


void main() {
    gl_Position = ubo.modelViewProj * vec4(inPosition, 1.0);
}