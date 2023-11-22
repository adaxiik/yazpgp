#version 330
layout(location=0) in vec3 vertex_position;
layout(location=1) in vec3 vertex_normal;
layout(location=2) in vec2 vertex_texcoord;
layout(location=3) in vec3 vertex_tangent;

out vec3 vs_normal;
out vec2 vs_texcoord;
out vec3 world_position;
out mat3 tbn_matrix;

uniform mat4 mvp_matrix;
uniform mat4 model_matrix;

void main () {
    gl_Position = mvp_matrix * vec4(vertex_position, 1.0);
    vs_normal = vertex_normal;
    vs_texcoord = vertex_texcoord;
    vec4 p = model_matrix * vec4(vertex_position, 1.0);
    world_position = p.xyz / p.w;

    vec3 t = normalize(vec3(model_matrix * vec4(vertex_tangent, 0.0)));
    vec3 n = normalize(vec3(model_matrix * vec4(vertex_normal, 0.0)));
    vec3 b = cross(n, t);
    tbn_matrix = mat3(t, b, n);
}