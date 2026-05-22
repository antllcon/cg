#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D imageTexture;
uniform int medianRadius;

void main()
{
    FragColor = texture(imageTexture, TexCoord);
}