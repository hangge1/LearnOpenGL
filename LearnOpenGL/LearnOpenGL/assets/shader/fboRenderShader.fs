#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{ 
    vec3 sColor = vec3(texture(screenTexture, TexCoords));
    vec3 inverseColor = vec3(1.0) - sColor; //反相

    float avg = (sColor.r + sColor.g + sColor.b) / 3.0f;
    vec3 greyColor = vec3(avg); //灰度化

    vec4 result = vec4(greyColor, 1.0f);
    FragColor = result;
}