#version 460 core

layout (vertices = 4) out;

uniform mat4 projection;
uniform mat4 view;

void main() {
    int segments = 255;

    gl_TessLevelOuter[0] = 1;
    gl_TessLevelOuter[1] = segments;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}