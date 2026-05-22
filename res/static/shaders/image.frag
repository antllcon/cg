#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D imageTexture;
uniform int useMedian;
uniform int medianRadius;

float GetLuminance(vec3 color)
{
    return dot(color, vec3(0.299, 0.587, 0.114));
}

void main()
{
    if (useMedian == 0 || medianRadius == 0)
    {
        FragColor = texture(imageTexture, TexCoord);
        return;
    }

    vec2 texelSize = 1.0 / vec2(textureSize(imageTexture, 0));

    vec4 colors[225];
    float lumas[225];

    int count = 0;

    for (int x = -medianRadius; x <= medianRadius; ++x)
    {
        for (int y = -medianRadius; y <= medianRadius; ++y)
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            vec4 pixelColor = texture(imageTexture, TexCoord + offset);

            colors[count] = pixelColor;
            lumas[count] = GetLuminance(pixelColor.rgb);
            count++;
        }
    }

    for (int i = 1; i < count; ++i)
    {
        vec4 keyColor = colors[i];
        float keyLuma = lumas[i];
        int j = i - 1;

        while (j >= 0 && lumas[j] > keyLuma)
        {
            colors[j + 1] = colors[j];
            lumas[j + 1] = lumas[j];
            j--;
        }

        colors[j + 1] = keyColor;
        lumas[j + 1] = keyLuma;
    }

    int medianIndex = count / 2;
    FragColor = colors[medianIndex];
}