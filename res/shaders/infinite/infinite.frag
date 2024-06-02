
#version 460 core

in vec3 nearPoint;
in vec3 farPoint;
in mat4 fragView;
in mat4 fragProj;
out vec4 fragColor;

uniform vec3 viewPos;
uniform float yLevel;
uniform float near;
uniform float far;
uniform vec3 u_Direction;

struct Material {
    bool hasTexture;
    sampler2D texture;
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

float computeDepth(vec3 pos) {
    vec4 clip_space_pos = fragProj * fragView * vec4(pos.xyz, 1.0);
    return (clip_space_pos.z / clip_space_pos.w);
}
float computeLinearDepth(vec3 pos) {
    vec4 clip_space_pos = fragProj * fragView * vec4(pos.xyz, 1.0);
    float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0; // put back between -1 and 1
    float linearDepth = (2.0 * near * far) / (far + near - clip_space_depth * (far - near)); // get linear value between 0.01 and 100
    return linearDepth / far; // normalize
}

void main() {
    float t = (yLevel-nearPoint.y) / (farPoint.y - nearPoint.y);
    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);
    gl_FragDepth = ((gl_DepthRange.diff * computeDepth(fragPos3D)) + gl_DepthRange.near + gl_DepthRange.far) / 2.0;

    float linearDepth = computeLinearDepth(fragPos3D);
    float fading = max(0, (1 - linearDepth));

    vec2 texCoords = vec2(fragPos3D.x, fragPos3D.z);

    vec4 objectColor = material.hasTexture? texture(material.texture, texCoords) : material.albedo;

    vec3 sunDir = normalize(u_Direction);
    float waterSunGradient = dot(normalize(viewPos), -normalize(sunDir));
    waterSunGradient = clamp(pow(waterSunGradient*0.7+0.3,2.0),0.0,1.0);
    vec3 waterSunColor = vec3(0.0,1.0,0.85)*waterSunGradient;
    waterSunColor = waterSunColor*0.5;

    float waterGradient = dot(normalize(viewPos), vec3(0.0,0.0,-1.0));
    waterGradient = clamp((waterGradient*0.5+0.5),0.2,1.0);
    vec3 scatteringColor = (vec3(0.0078, 0.5176, 0.700)+waterSunColor)*waterGradient*1.5;
    float depthColorScale = 1 / 20.f;
    float depth = length(fragPos3D - viewPos);

    vec3 color = mix(objectColor.rgb, scatteringColor/3.f, clamp(depth*depthColorScale/2, 0, 1));

    fragColor = vec4(color, objectColor.a * fading);
}
