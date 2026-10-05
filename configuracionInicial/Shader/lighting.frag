#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;

// Fuente de luz 1 (existente)
uniform Light light;

// Fuente de luz 2 (adicional con parametros diferentes)
uniform Light light2;

uniform sampler2D texture_diffuse1;

vec3 CalcPointLight(Light l, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // Ambient
    vec3 ambient = l.ambient * material.ambient;
    
    // Diffuse
    vec3 lightDir = normalize(l.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = l.diffuse * (diff * material.diffuse);
    
    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = l.specular * (spec * material.specular);
    
    return (ambient + diffuse + specular);
}

void main()
{
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    
    // Descarte de pixeles transparentes
    if (texColor.a < 0.1)
        discard;

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Suma de ambas fuentes de iluminacion
    vec3 result = CalcPointLight(light, norm, FragPos, viewDir);
    result += CalcPointLight(light2, norm, FragPos, viewDir);

    color = vec4(result, 1.0f) * texColor;
}