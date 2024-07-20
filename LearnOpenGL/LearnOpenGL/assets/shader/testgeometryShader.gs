#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in vec3 vColor[];

out vec3 fColor;

void Build_House(vec4 position)
{
    fColor = vColor[0];
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, -0.1, 0.0, 0.0); 
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.1, -0.1, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.1, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.1, 0.1, 0.0, 0.0);
    EmitVertex();

    fColor = vec3(1.0,1.0,1.0);
    gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.2, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}

void main()
{             
    Build_House(gl_in[0].gl_Position);
}