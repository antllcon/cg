#version 330 core

in vec2 textureCoordinate;
out vec4 fragColor;

uniform sampler2D imageTexture;
uniform int useMedian;
uniform int medianRadius;

uniform vec2 texelSize;
uniform int windowArea;
uniform int medianIndex;

const int MAX_SAMPLES = 225;
const vec3 LUMINANCE_WEIGHTS = vec3(0.299, 0.587, 0.114);

float CalculateLuminance(vec3 color)
{
    return dot(color, LUMINANCE_WEIGHTS);
}

vec4 FindMedianColor(vec4 colors[MAX_SAMPLES], float luminances[MAX_SAMPLES], int count, int medIndex)
{
    for (int i = 1; i < count; ++i)
    {
        vec4 keyColor = colors[i];
        float keyLuminance = luminances[i];
        int j = i - 1;

        while (j >= 0 && luminances[j] > keyLuminance)
        {
            colors[j + 1] = colors[j];
            luminances[j + 1] = luminances[j];
            j--;
        }

        colors[j + 1] = keyColor;
        luminances[j + 1] = keyLuminance;
    }

    return colors[medIndex];
}

void main()
{
    if (useMedian == 0 || medianRadius == 0)
    {
        fragColor = texture(imageTexture, textureCoordinate);
        return;
    }

    vec4 neighborColors[MAX_SAMPLES];
    float neighborLuminances[MAX_SAMPLES];

    int currentSample = 0;

    for (int x = -medianRadius; x <= medianRadius; ++x)
    {
        for (int y = -medianRadius; y <= medianRadius; ++y)
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            vec4 pixelColor = texture(imageTexture, textureCoordinate + offset);

            neighborColors[currentSample] = pixelColor;
            neighborLuminances[currentSample] = CalculateLuminance(pixelColor.rgb);
            currentSample++;
        }
    }

    fragColor = FindMedianColor(neighborColors, neighborLuminances, windowArea, medianIndex);
}