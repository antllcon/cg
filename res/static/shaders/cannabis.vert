#version 330 core

layout(location = 0) in vec2 position;

uniform mat4 projection;

const float BASE_SCALE = 0.35;
const vec2 CENTER_OFFSET = vec2(0.0, -0.3);

void main()
{
    float angle = position.x;

    float radius = (1.0 + sin(angle))
        * (1.0 + 0.9 * cos(8.0 * angle))
        * (1.0 + 0.1 * cos(24.0 * angle))
        * (0.5 + 0.05 * cos(140.0 * angle));

    vec2 curved = vec2(radius * cos(angle), radius * sin(angle));

    vec2 point = curved * BASE_SCALE + CENTER_OFFSET;

    gl_Position = projection * vec4(point, 0.0, 1.0);
}