#version 330 core

out vec4 FragColor;
in vec2 TextureCoordinates;

uniform sampler2D u_currentGlow;
uniform sampler2D u_historyGlow;
uniform float u_fadeFactor;

void main()
{
    vec3 currentColor = texture(u_currentGlow, TextureCoordinates).rgb;
    vec3 historyColor = texture(u_historyGlow, TextureCoordinates).rgb;

    vec3 finalColor = currentColor + (historyColor * u_fadeFactor);

    FragColor = vec4(finalColor, 1.0);
}