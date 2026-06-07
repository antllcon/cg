#version 330 core

layout (location = 0) in vec2 layoutPosition;
layout (location = 1) in vec2 layoutTexture;

out vec2 textureCoordinate;

uniform vec2 scale;
uniform int flipY;

void main()
{
    gl_Position = vec4(layoutPosition.x * scale.x, layoutPosition.y * scale.y, 0.0, 1.0);
    float texY = flipY == 1 ? 1.0 - layoutTexture.y : layoutTexture.y;
    textureCoordinate = vec2(layoutTexture.x, texY);
}
