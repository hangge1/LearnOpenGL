#version 330 core
layout (location = 0) in vec3 aPos;


uniform mat4 model;
uniform mat4 view;
uniform mat4 project;

out vec4 varyingColor;

void main()
{
    varyingColor = vec4(aPos, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
    gl_Position = project * view * model * vec4(aPos, 1.0);
}