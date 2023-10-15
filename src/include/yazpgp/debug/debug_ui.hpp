#pragma once
#include "scene.hpp"

namespace yazpgp
{
    class DebugUI
    {
        DebugUI() = default;
        ~DebugUI() = default;
        static void camera_component(Camera& camera);
        static void lights_component(std::vector<PointLight>& lights);
        static void entities_component(std::vector<std::unique_ptr<RenderableEntity>>& entities);
    public:
        static void scene_window(Scene& scene);
    };
}