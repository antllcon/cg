#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D u_mainTexture;
uniform sampler2D u_glowTexture;

void main()
{
    vec3 mainColor = texture(u_mainTexture, TexCoords).rgb;
    vec3 glowColor = texture(u_glowTexture, TexCoords).rgb;

    vec3 result = mainColor + glowColor;

    FragColor = vec4(result, 1.0);
}