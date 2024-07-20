#version 330 core
out vec4 FragColor;

//in vec3 Normal;
//in vec3 Position;

in VS_OUT
{
    vec3 Normal;
    vec3 Position;
} fs_in;

void main()
{             
    FragColor = vec4(abs(fs_in.Normal), 1.0);
}