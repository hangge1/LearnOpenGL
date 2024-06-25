#version 330 core

out vec4 FragColor;

in vec2 vTexCoord;

uniform sampler2D sample;

void main()
{
    //FragColor = vec4(vColor, 1.0f);

    FragColor = texture(sample, vTexCoord);
}