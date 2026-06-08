#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform vec4 u_color;
uniform bool u_isEmissive;

void main()
{
    FragColor = u_color;

    if (u_isEmissive)
    {
        BrightColor = u_color;
    }
    else
    {
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}