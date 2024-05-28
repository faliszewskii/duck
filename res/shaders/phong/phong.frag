#version 400 core
out vec4 fragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

uniform vec3 viewPos;

struct Material {
    bool hasTexture;
    sampler2D texture;
    float shininess;
    vec4 albedo;
};
uniform Material material;

uniform sampler2D anisotropicTexture;

struct DirLight {
    vec3 direction;
    vec3 color;
    float strength;
};
uniform DirLight dirLight;


vec3 getAnisoFromMap(vec3 normal)
{
    vec3 anisotropic = texture(anisotropicTexture, texCoords).rgb;// * 2.0 - 1.0;
    return anisotropic;

    vec3 Q1  = dFdx(fragPos);
    vec3 Q2  = dFdy(fragPos);
    vec2 st1 = dFdx(texCoords);
    vec2 st2 = dFdy(texCoords);

    vec3 N   = normalize(anisotropic);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    vec3 result = TBN * anisotropic;
    result = vec3(result.xy, result.z);
    return normalize(result);
}


vec3 calculateLight(DirLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = max(normalize(lightDir + viewDir), 0);

    // Anisotropic:
    //// HdotA is 0 when perpendicular to the Anisotropic normal map, 1 when parallel.
    float HdotA = dot(normalize(normal + getAnisoFromMap(normal)), halfwayDir);
    //// modify the value with sin() so that we get a darker
    //// middle highlight and a ring effect based off of the halfVector.
    float aniso = max(0, sin(radians((HdotA - 0.2) * 180)));

    // Specular:
    //// scale the aniso value by taking it to a power of s.Gloss,
    //// then globally decrease its strengthby multiplying it by s.Specular.


    float spec = material.shininess != 0 ? pow(aniso, material.shininess) *0.2: 0;
    // combine results
    vec3 ambient  = light.color * vec3(0.2f);
    vec3 diffuse  = light.color * diff;
    vec3 specular = light.color * spec;
    return ambient + (diffuse + specular) * light.strength;
}

void main()
{
    vec4 objectColor = material.hasTexture? texture(material.texture, texCoords) : material.albedo;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 normNormal = normalize(normal);
    vec3 result = vec3(0);

    result += calculateLight(dirLight, normNormal, fragPos, viewDir);
    result *= objectColor.rgb;

    result =  pow(result, vec3(1.0/2.2f));
    fragColor = vec4(result, objectColor.a);
}