#version 330 core

out vec4 FragColor;

in vec2 vTexCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

void main()
{
    //FragColor = vec4(vColor, 1.0f);

    FragColor = texture(sample, vTexCoord);
}