#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;



void main()
{             
    
    FragColor = vec4(abs(Normal), 1.0);
}