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
    float illumination_radius;
};

struct Material{
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
    float specular_shininess;
};


#define MAX_POINT_LIGHTS 4
uniform int num_point_lights;
uniform PointLight point_lights[MAX_POINT_LIGHTS];
uniform Material material;

uniform vec3 camera_position;
uniform mat3 normal_matrix;

void main () {
    // frag_color = vec4 (point_lights[0].color, 1.0f);
    // frag_color = texture(fs_tex0, vs_texcoord);
    vec3 self_color = vec3(1.0);
    
    PointLight light = point_lights[0];
    vec3 ambient_light = light.ambient_intensity * light.color * material.ambient_color;

    vec3 normal = normalize(normal_matrix * vs_normal);

    float distance = length(light.position - world_position);
    float illumination_factor = max(1.0f - distance / light.illumination_radius * 2.0f, 0.0f);

    vec3 light_direction = normalize(light.position - world_position);
    float diffuse_factor = max(dot(normal, light_direction), 0.0f);
    vec3 diffuse_light = diffuse_factor * light.diffuse_intensity * light.color * material.diffuse_color;

    vec3 view_direction = normalize(camera_position - world_position);
    vec3 halfway_direction = normalize(light_direction + view_direction);
    float specular_factor = pow(max(dot(normal, halfway_direction), 0.0f), material.specular_shininess);
    vec3 specular_light = specular_factor * light.specular_intensity * light.color * material.specular_color;


    frag_color = vec4(self_color * (ambient_light + (diffuse_light + specular_light) * illumination_factor), 1.0f);
    // frag_color = vec4(self_color * (specular_light), 1.0f);
}