#version 330 core
layout (location = 0) in vec3 aPos;


//uniform mat4 model;
uniform mat4 view;
uniform mat4 project;

uniform mat4 batchModels[16];

out vec4 varyingColor;



void main()
{
    varyingColor = vec4(aPos, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);

    gl_Position = project * view * batchModels[gl_InstanceID] * vec4(aPos, 1.0);
}