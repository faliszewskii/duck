#version 460 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

uniform mat4 view;
uniform mat4 projection;

uniform vec3 positionStart;
uniform vec3 positionEnd;

void main() {
    gl_Position = projection * view * vec4(positionStart, 1.0);
    EmitVertex();
    gl_Position = projection * view * vec4(positionEnd, 1.0);
    EmitVertex();
    EndPrimitive();
}