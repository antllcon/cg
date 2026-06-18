#version 330 core

layout(location = 0) in vec2 position;

const float BASE_SCALE = 0.35;

void main()
{
    gl_Position = vec4(position * BASE_SCALE + vec2(0.0, -0.3), 0.0, 1.0);
}