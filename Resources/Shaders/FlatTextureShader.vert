#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 modelView;
    mat4 modelViewProj;
} ubo;


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec2 outTexCoord;

void main() {
    gl_Position = ubo.modelViewProj * vec4(inPosition, 1.0);
    outTexCoord = inTexCoord;
}