#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
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
in vec2 vTexCoord; 

out vec4 FragColor;

uniform vec3 u_ViewPos;
uniform Light u_Light;
uniform Material u_Material;

void main()
{
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(u_Light.position - vPos);

    //环境光
    vec3 ambient = u_Light.ambient * vec3(texture(u_Material.diffuse,vTexCoord));

    //漫反射
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_Light.diffuse * diff * vec3(texture(u_Material.diffuse,vTexCoord));

    //镜面反射
    vec3 viewDir = normalize(u_ViewPos - vPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = u_Light.specular * spec * vec3(texture(u_Material.specular,vTexCoord));

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0f);
}