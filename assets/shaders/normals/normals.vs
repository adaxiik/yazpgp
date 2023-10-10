#version 330
layout(location=0) in vec3 vertex_position;
layout(location=1) in vec3 vertex_normal;
out vec3 vs_normal;
uniform mat4 mvp_matrix;
uniform mat4 model_matrix;

void main () {
    gl_Position = mvp_matrix * vec4(vertex_position, 1.0);
    vs_normal = vertex_normal;
}