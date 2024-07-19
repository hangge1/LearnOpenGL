#version 330 core
out vec4 FragColor;

in vec4 varyingColor;

void main()
{             
    FragColor = varyingColor;
}