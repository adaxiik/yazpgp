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

void main () {
    // frag_color = vec4 (point_lights[0].color, 1.0f);
    // frag_color = texture(fs_tex0, vs_texcoord);
    vec3 self_color = vec3(1.0, 0.0, 0.0);
    
    PointLight light = point_lights[0];
    vec3 ambient_light = light.ambient_intensity * light.color;

    vec3 normal = vs_normal;
    vec3 light_direction = normalize(light.position - world_position);
    float diffuse_factor = max(dot(normal, light_direction), 0.0f);
    vec3 diffuse_light = diffuse_factor * light.diffuse_intensity * light.color;

    vec3 view_direction = normalize(camera_position - world_position);
    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular_factor = pow(max(dot(view_direction, reflect_direction), 0.0f), 16.0f);
    // float specular_factor = max(dot(view_direction, reflect_direction), 0.0f);
    vec3 specular_light = specular_factor * light.specular_intensity * light.color;


    frag_color = vec4(self_color * (ambient_light + diffuse_light + specular_light), 1.0f);
    // frag_color = vec4(self_color * (specular_light), 1.0f);
}