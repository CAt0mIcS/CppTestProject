#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec4 outColor;

const vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
const vec3 objectColor = vec3(1.0f, 1.0f, 1.0f);
const vec3 lightPos = vec3(0.0f, 0.0f, 0.0f);

const float ambientStrength = 0.1;


void main()
{
    vec3 fragPos = lightPos - inWorldPos;

    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(inNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    outColor = vec4(result, 1.0);

}