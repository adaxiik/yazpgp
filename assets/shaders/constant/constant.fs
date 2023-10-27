#version 330
out vec4 frag_color;
in vec3 vs_normal;
in vec2 vs_texcoord;
in vec3 world_position;

uniform sampler2D fs_tex0;


struct Material{
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
    float specular_shininess;
};


uniform Material material;

uniform mat3 normal_matrix;

void main () {
    // frag_color = vec4 (point_lights[0].color, 1.0f);
    // frag_color = texture(fs_tex0, vs_texcoord);

    frag_color = vec4(material.ambient_color , 1.0f);
    // frag_color = vec4(self_color * (specular_light), 1.0f);
}