#version 330
out vec4 frag_color;
in vec3 vs_normal;
in vec2 vs_texcoord;
in vec3 world_position;

uniform sampler2D fs_tex0;

// ********** LIGHTNING **********

struct Material{
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
    float specular_shininess;
};

struct Intensity{
    float ambient;
    float diffuse;
    float specular;
};

struct PointLight{
    vec3 position;
    vec3 color;
    Intensity intensity;
    float illumination_radius;
};

struct DirectionalLight{
    vec3 direction;
    vec3 color;
    Intensity intensity;
};

struct SpotLight{
    vec3 position;
    vec3 direction;
    vec3 color;
    Intensity intensity;
    float illumination_radius;
    float inner_cone_angle_degrees;
    float outer_cone_angle_degrees;
};

#define MAX_POINT_LIGHTS 4
#define MAX_DIRECTIONAL_LIGHTS 1
#define MAX_SPOT_LIGHTS 4

struct Light{
    PointLight point_lights[MAX_POINT_LIGHTS];
    DirectionalLight directional_lights[MAX_DIRECTIONAL_LIGHTS];
    SpotLight spot_lights[MAX_SPOT_LIGHTS];
    int num_point_lights;
    int num_directional_lights;
    int num_spot_lights;
};

uniform Light light;
uniform Material material;

vec3 point_light_ambient(PointLight light){
    return light.intensity.ambient * light.color * material.ambient_color;
}

vec3 point_light_diffuse(PointLight light, vec3 normal, vec3 light_direction){
    float diffuse_factor = max(dot(normal, light_direction), 0.0f);
    return diffuse_factor * light.intensity.diffuse * light.color * material.diffuse_color;
}

vec3 point_light_specular(PointLight light, vec3 normal, vec3 light_direction, vec3 view_direction){
    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular_factor = pow(max(dot(view_direction, reflect_direction), 0.0f), material.specular_shininess);
    // if (diffuse_factor == 0.0f) {
        // specular_factor = 0.0f;
    // }
    return specular_factor * light.intensity.specular * light.color * material.specular_color;
}

float point_light_attenuation(PointLight light, vec3 world_position){
    float distance = length(light.position - world_position);
    return max(1.0f - distance / light.illumination_radius * 2.0f, 0.0f);
}

vec3 point_light(PointLight light, vec3 normal, vec3 light_direction, vec3 view_direction, vec3 world_position){
    vec3 ambient_light = point_light_ambient(light);
    vec3 diffuse_light = point_light_diffuse(light, normal, light_direction);
    vec3 specular_light = point_light_specular(light, normal, light_direction, view_direction);
    float attenuation = point_light_attenuation(light, world_position);

    return attenuation * (ambient_light + diffuse_light + specular_light);
}

vec3 directional_light_ambient(DirectionalLight light){
    return light.intensity.ambient * light.color * material.ambient_color;
}

vec3 directional_light_diffuse(DirectionalLight light, vec3 normal){
    float diffuse_factor = max(dot(normal, -light.direction), 0.0f);
    return diffuse_factor * light.intensity.diffuse * light.color * material.diffuse_color;
}

vec3 directional_light_specular(DirectionalLight light, vec3 normal, vec3 view_direction){
    vec3 reflect_direction = reflect(light.direction, normal);
    float specular_factor = pow(max(dot(view_direction, reflect_direction), 0.0f), material.specular_shininess);
    // if (diffuse_factor == 0.0f) {
        // specular_factor = 0.0f;
    // }
    return specular_factor * light.intensity.specular * light.color * material.specular_color;
}

vec3 directional_light(DirectionalLight light, vec3 normal, vec3 view_direction){
    vec3 ambient_light = directional_light_ambient(light);
    vec3 diffuse_light = directional_light_diffuse(light, normal);
    vec3 specular_light = directional_light_specular(light, normal, view_direction);

    return ambient_light + diffuse_light + specular_light;
}


vec3 spot_light_ambient(SpotLight light){
    return light.intensity.ambient * light.color * material.ambient_color;
}

vec3 spot_light_diffuse(SpotLight light, vec3 normal, vec3 light_direction){
    float diffuse_factor = max(dot(normal, light_direction), 0.0f);
    return diffuse_factor * light.intensity.diffuse * light.color * material.diffuse_color;
}

vec3 spot_light_specular(SpotLight light, vec3 normal, vec3 light_direction, vec3 view_direction){
    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular_factor = pow(max(dot(view_direction, reflect_direction), 0.0f), material.specular_shininess);
    // if (diffuse_factor == 0.0f) {
        // specular_factor = 0.0f;
    // }
    return specular_factor * light.intensity.specular * light.color * material.specular_color;
}

float spot_light_attenuation(SpotLight light, vec3 world_position){
    float distance = length(light.position - world_position);
    return max(1.0f - distance / light.illumination_radius * 2.0f, 0.0f);
}

float spot_light_cone(SpotLight light, vec3 light_direction){
    float cos_inner_cone_angle = cos(radians(light.inner_cone_angle_degrees));
    float cos_outer_cone_angle = cos(radians(light.outer_cone_angle_degrees));
    float cos_angle = dot(light.direction, -light_direction);
    if (cos_angle >= cos_inner_cone_angle) {
        return 1.0f;
    } else if (cos_angle < cos_outer_cone_angle) {
        return 0.0f;
    } else {
        // return pow((cos_angle - cos_outer_cone_angle) / (cos_inner_cone_angle - cos_outer_cone_angle), 2.0f);
        return (cos_angle - cos_outer_cone_angle) / (cos_inner_cone_angle - cos_outer_cone_angle);
    }
}

vec3 spot_light(SpotLight light, vec3 normal, vec3 light_direction, vec3 view_direction, vec3 world_position){
    vec3 ambient_light = spot_light_ambient(light);
    vec3 diffuse_light = spot_light_diffuse(light, normal, light_direction);
    vec3 specular_light = spot_light_specular(light, normal, light_direction, view_direction);
    float attenuation = spot_light_attenuation(light, world_position);
    float cone = spot_light_cone(light, light_direction);

    return attenuation * cone * (ambient_light + diffuse_light + specular_light);
}

vec3 all_lights(vec3 normal, vec3 view_direction, vec3 world_position){
    vec3 light_color = vec3(0.0f);
    for (int i = 0; i < light.num_point_lights; i++) {
        vec3 light_direction = normalize(light.point_lights[i].position - world_position);
        light_color += point_light(light.point_lights[i], normal, light_direction, view_direction, world_position);
    }
    for (int i = 0; i < light.num_directional_lights; i++) {
        light_color += directional_light(light.directional_lights[i], normal, view_direction);
    }
    for (int i = 0; i < light.num_spot_lights; i++) {
        vec3 light_direction = normalize(light.spot_lights[i].position - world_position);
        light_color += spot_light(light.spot_lights[i], normal, light_direction, view_direction, world_position);
    }

    return light_color;
}


// ********** LIGHTNING **********

uniform vec3 camera_position;
uniform mat3 normal_matrix;

void main () {
    vec3 self_color = texture(fs_tex0, vs_texcoord).xyz;
    
    vec3 normal = normalize(normal_matrix * vs_normal);
    vec3 view_direction = normalize(camera_position - world_position);
    vec3 light_color = all_lights(normal, view_direction, world_position);

    frag_color = vec4(self_color * light_color, 1.0f);
}