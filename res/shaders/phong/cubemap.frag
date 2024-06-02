#version 400 core
out vec4 fragColor;

in vec3 normal;
in vec3 fragPos;
in vec3 texCoords;

uniform vec3 viewPos;

struct Material {
    float shininess;
    vec4 albedo;

    bool hasTexture;
    samplerCube texture;
};
uniform Material material;

struct DirLight {
    vec3 direction;
    vec3 color;
    float strength;
};
uniform DirLight dirLight;

vec3 calculateLight(DirLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = max(normalize(lightDir + viewDir), 0);
    float spec = material.shininess != 0 ? pow(max(dot(normal, halfwayDir), 0.0), material.shininess) : 0;
    // combine results
    vec3 ambient  = light.color * vec3(0.2f);
    vec3 diffuse  = light.color * diff;
    vec3 specular = light.color * spec;
    return ambient + (diffuse + specular) * light.strength;
}

void main()
{
    vec4 objectColor = texture(material.texture, texCoords);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 normNormal = normalize(normal);
    vec3 result = vec3(0);

    result += calculateLight(dirLight, normNormal, fragPos, viewDir);
    result *= objectColor.rgb;

    float gamma = 2.2f;
    result =  pow(result, vec3(1.0/gamma));

    fragColor = vec4(result, objectColor.a);
}