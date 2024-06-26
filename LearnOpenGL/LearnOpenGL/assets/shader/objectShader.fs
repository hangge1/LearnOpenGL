#version 330 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light 
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 vNormal;
in vec3 vPos;

out vec4 FragColor;

uniform vec3 u_ViewPos;
uniform Light u_Light;
uniform Material u_Material;

void main()
{
    //环境光
    vec3 ambient = u_Light.ambient * u_Material.ambient;

    //漫反射
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(u_Light.position - vPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_Light.diffuse * (diff * u_Material.diffuse);

    //镜面反射
    vec3 viewDir = normalize(u_ViewPos - vPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = u_Light.specular * (spec * u_Material.specular);

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0f);
}