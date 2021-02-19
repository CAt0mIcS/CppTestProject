#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform Lighting
{
    vec3 lightColor;
    vec3 lightPos;
} uLight;


layout(location = 0) in vec3 inViewPos;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec4 outColor;


const vec3 materialColor = { 0.7f,0.7f,0.9f };
const vec3 ambient = { 0.05f,0.05f,0.05f };
const vec3 diffuseColor = { 1.0f,1.0f,1.0f };
const float diffuseIntensity = 1.0f;
const float attConst = 1.0f;
const float attLin = 0.045f;
const float attQuad = 0.0075f;

const float specularIntensity = 1.0f;
const float specularPower = 1.5f;


void main()
{
	const vec3 viewNormal = normalize(inNormal);

    // fragment to light vector data
	const vec3 vToL = uLight.lightPos - inViewPos;
	const float distToL = length(vToL);
	const vec3 dirToL = vToL / distToL;

	// diffuse attenuation
	const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));

	// diffuse intensity
	const vec3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, viewNormal));

	// reflected light vector
    const vec3 w = viewNormal * dot(vToL, viewNormal);
    const vec3 r = w * 2.0f - vToL;

	// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    const vec3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(inViewPos))), specularPower);

	// final color
	// return vec4(saturate((diffuse + ambient) * materialColor), 1.0f);

	vec3 result = clamp((diffuse + ambient) * materialColor + specular, 0.0f, 1.0f);
	outColor = vec4(result, 1.0f);
}