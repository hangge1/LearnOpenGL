#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Project;

void main()
{
    gl_Position = u_Project * u_View * u_Model * vec4(aPos, 1.0);
};