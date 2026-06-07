#version 330 core

in vec2 textureCoordinate;
out vec4 fragmentColor;

uniform sampler2D imageTexture;
uniform vec2 direction;
uniform float weights[21];
uniform int kernelRadius;

const float GAMMA = 2.2;
const float INV_GAMMA = 1.0 / 2.2;

vec3 ToLinear(vec3 srgb)
{
    return pow(srgb, vec3(GAMMA));
}

vec3 ToSRGB(vec3 linear)
{
    return pow(linear, vec3(INV_GAMMA));
}

void main()
{
    vec3 result = ToLinear(texture(imageTexture, textureCoordinate).rgb) * weights[0];

    for (int i = 1; i <= kernelRadius; i++)
    {
        vec2 offset = direction * float(i);
        result += ToLinear(texture(imageTexture, textureCoordinate + offset).rgb) * weights[i];
        result += ToLinear(texture(imageTexture, textureCoordinate - offset).rgb) * weights[i];
    }

    fragmentColor = vec4(ToSRGB(result), 1.0);
}
