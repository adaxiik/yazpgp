#pragma once
#include "point_light.hpp"
#include "spot_light.hpp"
#include "directional_light.hpp"
#include "shader.hpp"
#include <variant>
namespace yazpgp
{
    using Light = std::variant<PointLight, SpotLight, DirectionalLight>;

    struct LightUseVisitor
    {
        const Shader& shader;
        const size_t index = 0;

        void operator()(const PointLight& point_light)
        {
            shader.set_uniform("light.point_lights[" + std::to_string(index) + "].position", point_light.position);
            shader.set_uniform("light.point_lights[" + std::to_string(index) + "].color", point_light.color);
            shader.set_uniform("light.point_lights[" + std::to_string(index) + "].intensity.ambient", point_light.ambient_intensity);
            shader.set_uniform("light.point_lights[" + std::to_string(index) + "].intensity.diffuse", point_light.diffuse_intensity);
            shader.set_uniform("light.point_lights[" + std::to_string(index) + "].intensity.specular", point_light.specular_intensity);
            shader.set_uniform("light.point_lights[" + std::to_string(index) + "].illumination_radius", point_light.illumination_radius);
        }

        void operator()(const SpotLight& spot_light)
        {
            shader.set_uniform("light.spot_lights[" + std::to_string(index) + "].position", spot_light.position);
            shader.set_uniform("light.spot_lights[" + std::to_string(index) + "].direction", spot_light.direction);
            shader.set_uniform("light.spot_lights[" + std::to_string(index) + "].color", spot_light.color);
            shader.set_uniform("light.spot_lights[" + std::to_string(index) + "].intensity.ambient", spot_light.ambient_intensity);
            shader.set_uniform("light.spot_lights[" + std::to_string(index) + "].intensity.diffuse", spot_light.diffuse_intensity);
            shader.set_uniform("light.spot_lights[" + std::to_string(index) + "].intensity.specular", spot_light.specular_intensity);
            shader.set_uniform("light.spot_lights[" + std::to_string(index) + "].illumination_radius", spot_light.illumination_radius);
            shader.set_uniform("light.spot_lights[" + std::to_string(index) + "].inner_cone_angle_degrees", spot_light.inner_cone_angle_degrees);
            shader.set_uniform("light.spot_lights[" + std::to_string(index) + "].outer_cone_angle_degrees", spot_light.outer_cone_angle_degrees);
        }

        void operator()(const DirectionalLight& directional_light)
        {
            shader.set_uniform("light.directional_lights[" + std::to_string(index) + "].direction", directional_light.direction);
            shader.set_uniform("light.directional_lights[" + std::to_string(index) + "].color", directional_light.color);
            shader.set_uniform("light.directional_lights[" + std::to_string(index) + "].intensity.ambient", directional_light.ambient_intensity);
            shader.set_uniform("light.directional_lights[" + std::to_string(index) + "].intensity.diffuse", directional_light.diffuse_intensity);
            shader.set_uniform("light.directional_lights[" + std::to_string(index) + "].intensity.specular", directional_light.specular_intensity);
        }
        
    };
}