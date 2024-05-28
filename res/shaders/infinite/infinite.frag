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

    float depthColorScale = 1 / 20.f;
    vec3 scatteringColor = vec3(0,1.0,0.95)/3.f;
    float depth = length(fragPos3D - viewPos);
    vec3 color = mix(objectColor.rgb, scatteringColor, clamp(depth*depthColorScale, 0, 1));

    fragColor = vec4(color, objectColor.a * fading);
}
