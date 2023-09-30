#version 330
layout(location=0) in vec3 vertex_position;
layout(location=1) in vec3 vertex_normal;
layout(location=2) in vec2 vertex_texcoord;
out vec3 vs_normal;
out vec2 vs_texcoord;

uniform mat4 mvp;

void main () {
    gl_Position = mvp * vec4(vertex_position, 1.0);
    vs_normal = vertex_normal;
    vs_texcoord = vertex_texcoord;
}