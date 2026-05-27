#version 330 core
out vec4 FragColor;

uniform vec2 u_resolution;
uniform vec2 u_offset;
uniform float u_zoom;

vec3 palette(in float t)
{
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.0, 0.33, 0.67);
    return a + b * cos(6.28318 * (c * t + d));
}

void main()
{
    vec2 uv = (gl_FragCoord.xy - 0.5 * u_resolution) / u_resolution.y;

    vec2 c = uv / u_zoom + u_offset + vec2(-0.5, 0.0);

    vec2 z = vec2(0.0);
    int iter;
    const int MAX_ITER = 256;

    for (iter = 0; iter < MAX_ITER; iter++)
    {
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
        if (dot(z, z) > 256.0) break;
    }

    if (iter == MAX_ITER)
    {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else
    {
        float smooth_iter = float(iter) + 1.0 - log(log(length(z))) / log(2.0);

        float color_index = smooth_iter / float(MAX_ITER);
        vec3 col = palette(color_index * 5.0 + 0.5);

        FragColor = vec4(col, 1.0);
    }
}