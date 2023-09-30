#version 330
out vec4 frag_colour;
in vec3 vs_normal;
in vec2 vs_texcoord;
uniform sampler2D fs_tex0;
void main () {
    // frag_colour = vec4 (vs_normal, 1.0);

    frag_colour = texture(fs_tex0, vs_texcoord);
}