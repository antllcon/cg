#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

struct Material {
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

struct Light {
    int type;
    vec4 color;
    vec3 position;
    vec3 direction;
    float intensity;
};

#define MAX_LIGHTS 8

uniform Material u_Material;
uniform Light u_Lights[MAX_LIGHTS];
uniform int u_LightCount;
uniform vec3 u_ViewPos;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 result = vec3(0.0);

    for(int i = 0; i < u_LightCount; i++)
    {
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * u_Lights[i].color.rgb * u_Material.diffuse.rgb;

        vec3 lightDir;
        if (u_Lights[i].type == 0) {
            lightDir = normalize(-u_Lights[i].direction);
        } else {
            lightDir = normalize(u_Lights[i].position - FragPos);
        }

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * u_Lights[i].color.rgb * u_Material.diffuse.rgb;

        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
        vec3 specular = spec * u_Lights[i].color.rgb * u_Material.specular.rgb;

        result += (ambient + diffuse + specular) * u_Lights[i].intensity;
    }

    FragColor = vec4(result, u_Material.diffuse.a);
}