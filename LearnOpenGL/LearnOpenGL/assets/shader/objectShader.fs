#version 330 core

in vec3 vNormal;
in vec3 vPos;

out vec4 FragColor;

uniform vec3 u_objectColor;
uniform vec3 u_lightColor;
uniform vec3 u_lightPos;
uniform vec3 u_ViewPos;

void main()
{
    //环境光
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_lightColor;

    //漫反射
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(u_lightPos - vPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_lightColor;

    //镜面反射
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_ViewPos - vPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = specularStrength * spec * u_lightColor;

    vec3 result = (ambient + diffuse + specular) * u_objectColor;

    FragColor = vec4(result, 1.0f);
}