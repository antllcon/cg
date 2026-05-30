#version 330 core

in vec2 textureCoordinate;
out vec4 fragmentColor;

uniform sampler2D imageTexture;
uniform int useMedian;
uniform int medianRadius;

uniform vec2 texelSize;
uniform int medianArea;
uniform int medianIndex;

const int MAX_SAMPLES = 225;
const vec3 BRIGHT_WEIGHTS = vec3(0.299, 0.587, 0.114);

float CalculateBright(vec3 color)
{
    return dot(color, BRIGHT_WEIGHTS);
}

vec4 FindMedianColor(vec4 colors[MAX_SAMPLES], float brights[MAX_SAMPLES], int count, int medIndex)
{
    for (int i = 1; i < count; ++i)
    {
        vec4 keyColor = colors[i];
        float keyBright = brights[i];
        int j = i - 1;

        while (j >= 0 && brights[j] > keyBright)
        {
            colors[j + 1] = colors[j];
            brights[j + 1] = brights[j];
            j--;
        }

        colors[j + 1] = keyColor;
        brights[j + 1] = keyBright;
    }

    return colors[medIndex];
}

void main()
{
    if (useMedian == 0 || medianRadius == 0)
    {
        fragmentColor = texture(imageTexture, textureCoordinate);
        return;
    }

    vec4 neighborColors[MAX_SAMPLES];
    float neighborBrights[MAX_SAMPLES];

    int currentSample = 0;

    for (int x = -medianRadius; x <= medianRadius; ++x)
    {
        for (int y = -medianRadius; y <= medianRadius; ++y)
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            vec4 pixelColor = texture(imageTexture, textureCoordinate + offset);

            neighborColors[currentSample] = pixelColor;
            neighborBrights[currentSample] = CalculateBright(pixelColor.rgb);
            currentSample++;
        }
    }

    fragmentColor = FindMedianColor(neighborColors, neighborBrights, windowArea, medianIndex);
}