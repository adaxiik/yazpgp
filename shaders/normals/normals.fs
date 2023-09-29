#version 330
out vec4 frag_colour;
in vec3 vs_normal;
void main () {
    frag_colour = vec4 (vs_normal, 1.0);
}