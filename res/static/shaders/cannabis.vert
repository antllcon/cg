#version 330 core

layout(location = 0) in vec2 aPos;

const float BASE_SCALE = 0.35;

void main()
{
    gl_Position = vec4(aPos * BASE_SCALE, 0.0, 1.0);
}