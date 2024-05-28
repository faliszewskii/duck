#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 direction;

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    vec3 p = vec3(model * vec4(aPos, 1));
//    nearPoint = UnprojectPoint(p.x, p.y, 0.0, view, projection).xyz; // unprojecting on the near plane
    vec3 farPoint = UnprojectPoint(p.x, p.y, 1.0, view, projection).xyz; // unprojecting on the far plane
//    fragView = view;
//    fragProj = projection;
    direction = normalize(farPoint);
    gl_Position = vec4(p, 1.0); // using directly the clipped coordinates
}