#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(binding = 1) uniform Lighting
{
    vec3 lightPos;
    vec3 viewPos;
    vec3 lightColor;
    vec3 objectColor;
} uLight;

layout(location = 0) out vec4 FragColor;

layout(location = 0) in vec3 Normal;
layout(location = 1) in vec3 FragPos;


void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * uLight.lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(uLight.lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLight.lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(uLight.viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * uLight.lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * uLight.objectColor;
    FragColor = vec4(result, 1.0);
} 