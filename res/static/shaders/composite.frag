#version 330 core

out vec4 FragColor;

in vec2 TextureCoordinates;

uniform sampler2D u_mainTexture;
uniform sampler2D u_glowTexture;

void main()
{
    vec3 mainColor = texture(u_mainTexture, TextureCoordinates).rgb;
    vec3 glowColor = texture(u_glowTexture, TextureCoordinates).rgb;

    vec3 result = mainColor + glowColor;

    FragColor = vec4(result, 1.0);
}