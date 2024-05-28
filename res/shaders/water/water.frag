#version 400 core
out vec4 fragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

uniform vec3 viewPos;
uniform sampler2D normalMap;
uniform sampler3D perlinNoise;
uniform sampler2D seaFloorTexture;
uniform bool debugNormal;
uniform float time;
uniform float deepness;

struct Perlin {
    float amplitude;
    float frequency;
    float speed;
    float length;
};
uniform Perlin perlin;

struct Material {
    float shininess;
    vec4 albedo;
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
    vec3 diffuse  = light.color * diff;
    vec3 specular = light.color * spec;
    return (diffuse + specular) * light.strength;
}

vec3 perlinNormal(vec2 texCoords) {
    vec2 tex = texCoords + vec2(time*perlin.speed);
    float perlinX = texture(perlinNoise, vec3(tex/perlin.length, time/perlin.frequency)).r;
    float perlinZ = texture(perlinNoise, vec3(tex/perlin.length+0.5f, time/perlin.frequency)).r;
    perlinX = 2 * perlinX - 1;
    perlinZ = 2 * perlinZ - 1;
    return normalize(vec3(perlinX*perlin.amplitude, 10.f, perlinZ*perlin.amplitude));
}

float fresnel(float F0, float cosNV) {
    return F0 + (1 - F0) * pow((1 - cosNV), 5);
}



// ------------------------------------------------------------------
// UNIFORMS ---------------------------------------------------------
// ------------------------------------------------------------------

uniform vec3 u_Direction;
uniform vec3 p_A, p_B, p_C, p_D, p_E, p_Z;

// ------------------------------------------------------------------
// FUNCTIONS --------------------------------------------------------
// ------------------------------------------------------------------

vec3 perez(float cos_theta, float gamma, float cos_gamma, vec3 A, vec3 B, vec3 C, vec3 D, vec3 E)
{
    return (1 + A * exp(B / (cos_theta + 0.01))) * (1 + C * exp(D * gamma) + E * cos_gamma * cos_gamma);
}

// ------------------------------------------------------------------

vec3 preetham_sky_rgb(vec3 v, vec3 sun_dir)
{
    if(dot(v, sun_dir)>0.9999)
        return vec3(1,1,1);
    float cos_theta = clamp(v.y, 0, 1);
    float cos_gamma = dot(v, sun_dir);
    float gamma = acos(cos_gamma);

    vec3 R_xyY = p_Z * perez(cos_theta, gamma, cos_gamma, p_A, p_B, p_C, p_D, p_E);

    vec3 R_XYZ = vec3(R_xyY.x, R_xyY.y, 1 - R_xyY.x - R_xyY.y) * R_xyY.z / R_xyY.y;

    // Radiance
    float r = dot(vec3( 3.240479, -1.537150, -0.498535), R_XYZ);
    float g = dot(vec3(-0.969256,  1.875992,  0.041556), R_XYZ);
    float b = dot(vec3( 0.055648, -0.204043,  1.057311), R_XYZ);

    return vec3(r, g, b);
}

vec3 waterColor(vec3 normal, vec3 viewDir, vec3 fragPos, vec3 albedo) {
    vec3 sunDir = normalize(u_Direction);

    float F0 = pow(1/7.f, 2/2.2f);

    //    float cosNV = dot(normal, viewDir);
    bool underwater = viewPos.y<0;//cosNV < 0;
    if(underwater) normal = -normal;

    float rfrCoeff = underwater ? 1.33f : 0.75f;
    vec3 rfr = refract(-viewDir, normal, rfrCoeff);
    bool fullReflect = abs(rfr.x)<=0.0001 && abs(rfr.y)<=0.0001 && abs(rfr.z)<=0.0001;
    rfr = normalize(rfr);
    float tRfr = (-deepness - fragPos.y) / (rfr.y);
    vec3 intersection = fragPos + tRfr * rfr;
    vec3 refractedColourSky = fullReflect ? vec3(0) : preetham_sky_rgb(normalize(rfr), sunDir);
    vec3 rfrColor = underwater ? refractedColourSky: texture(seaFloorTexture, intersection.xz).rgb;

    vec3 rfl = reflect(-viewDir, normal);
    rfl = normalize(rfl);
    float tRfl = (-deepness - fragPos.y) / (rfl.y);
    intersection = fragPos + tRfl * rfl;
    vec3 rflColor = underwater ? texture(seaFloorTexture, intersection.xz).rgb : preetham_sky_rgb(rfl, sunDir);

    float fresnelCoeff = fullReflect ? 1 : fresnel(F0, max(dot(normal, viewDir), 0));


    // Specular reflection of the sun.
    float spec = clamp(pow(atan(max(dot(rfl, sunDir),0.0)*1.55),1000.0)*8.0,0.0,1.0);
    vec3 sunext = vec3(0.45, 0.55, 0.68);//sunlight extinction
    vec3 specColor = mix(vec3(1.0,0.5,0.2), vec3(1.0,1.0,1.0), clamp(1.0-exp(-(sunDir.z/500.0)*sunext),0.0,1.0));

    // Water color
    float waterSunGradient = dot(normalize(viewPos), -normalize(sunDir));
    waterSunGradient = clamp(pow(waterSunGradient*0.7+0.3,2.0),0.0,1.0);
    vec3 waterSunColor = vec3(0.0,1.0,0.85)*waterSunGradient;
    waterSunColor = (underwater) ? waterSunColor*0.5:waterSunColor*0.25;

    float waterGradient = dot(normalize(viewPos), vec3(0.0,0.0,-1.0));
    waterGradient = clamp((waterGradient*0.5+0.5),0.2,1.0);
    vec3 watercolor = (vec3(0.0078, 0.5176, 0.700)+waterSunColor)*waterGradient*1.5;
    vec3 waterext = vec3(0.6, 0.8, 1.0);//water extinction
    float sunFade = clamp((sunDir.z+10.0)/20.0,0.0,1.0);
    watercolor = mix(watercolor*0.3*sunFade, watercolor, clamp(1.0-exp(-(sunDir.z/500.0)*sunext),0.0,1.0));

    float depthColorScale = 1 / 10.f;
    vec3 scatteringColor = watercolor;
    if(underwater) {
        float depth = length(tRfl * rfl);
        rflColor = mix(rflColor, scatteringColor/3.f, clamp(depth*depthColorScale, 0, 1));
    } else {
        float depth = length(tRfr * rfr);
        rfrColor = mix(rfrColor, scatteringColor, clamp(depth*depthColorScale, 0, 1));
    }

    //    return vec3(rfl);//mix(rfrColor, rflColor, fresnelCoeff);
    return mix(rfrColor, rflColor, fresnelCoeff*0.8) + spec * specColor;
}

vec3 windWaves(vec2 texCoords) {
    float windAngle = 1.2;
    float t = texCoords.x * cos(windAngle) + texCoords.y * sin(windAngle);
    float z = cos(t*10+time) + cos(t*17+time)/2 + sin(t*15+time)/4  + sin(t*31+time)/2;
    vec3 tangentX = normalize(vec3(1, 0, 0));
    vec3 tangentZ = normalize(vec3(0, z/20, 1));

    vec3 normal = normalize(cross(tangentZ, tangentX));

    return normalize(normal);
}

void main()
{
    vec4 albedo = material.albedo;
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 normNormal = normalize(texture(normalMap, texCoords).rgb);

    vec2 tex = texCoords - 0.5;
    tex = vec2(tex.x,-tex.y);
    normNormal = normalize(normNormal + perlinNormal(tex));
//    normNormal = normalize(normNormal + windWaves(tex));

    vec3 result = vec3(0);
    result += waterColor(normNormal, viewDir, fragPos, albedo.rgb);
//    result += calculateLight(dirLight, normNormal, fragPos, viewDir) * albedo.rgb * 0.6;

    fragColor = vec4(result, albedo.w);

    if(debugNormal) fragColor = vec4(clamp(normNormal, 0, 1), 1);
}