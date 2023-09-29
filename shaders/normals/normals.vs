#version 330
layout(location=0) in vec3 vertex_position;
layout(location=1) in vec3 vertex_normal;
out vec3 vs_normal;
uniform mat4 mvp;

void main () {
    gl_Position = mvp * vec4(vertex_position, 1.0);
    vs_normal = vertex_normal;
}