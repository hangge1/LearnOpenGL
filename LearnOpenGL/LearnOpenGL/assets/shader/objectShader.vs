#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Project;

out vec3 vNormal;
out vec3 vPos;  
out vec2 vTexCoord;  

void main()
{
    vPos = vec3(u_Model * vec4(aPos, 1.0));
    vNormal = vec3(u_Model * vec4(aNormal, 0.0f)); 
    vTexCoord = aTexCoord;
    gl_Position = u_Project * u_View * vec4(vPos, 1.0);
};