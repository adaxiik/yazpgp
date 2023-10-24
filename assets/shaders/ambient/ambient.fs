#version 330
out vec4 frag_color;
in vec3 vs_normal;
in vec2 vs_texcoord;
in vec3 world_position;

uniform sampler2D fs_tex0;

struct PointLight{
    vec3 position;
    vec3 color;
    float ambient_intensity;
    float diffuse_intensity;
    float specular_intensity;
};


#define MAX_POINT_LIGHTS 4
uniform int num_point_lights;
uniform PointLight point_lights[MAX_POINT_LIGHTS];

uniform vec3 camera_position;
uniform mat3 normal_matrix;

void main () {
    // frag_color = vec4 (point_lights[0].color, 1.0f);
    // frag_color = texture(fs_tex0, vs_texcoord);
    vec3 self_color = vec3(1.0);
    
    PointLight light = point_lights[0];
    vec3 ambient_light = light.ambient_intensity * light.color;

    frag_color = vec4(self_color * ambient_light , 1.0f);
    // frag_color = vec4(self_color * (specular_light), 1.0f);
}