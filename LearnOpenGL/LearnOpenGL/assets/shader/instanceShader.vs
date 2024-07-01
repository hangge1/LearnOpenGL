#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec2 offsets[100];

void main()
{
    vColor =  aColor;

    vec2 offset = offsets[gl_InstanceID];
    gl_Position = projection * view * model * vec4(aPos + offset, 0.0f, 1.0f);

}