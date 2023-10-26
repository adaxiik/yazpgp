#include "debug/debug_ui.hpp"

#include <imgui/imgui.h>
// #include <imgui/backends/imgui_impl_sdl2.h>
// #include <imgui/backends/imgui_impl_opengl3.h>

#include <glm/glm.hpp>

namespace yazpgp
{
    void DebugUI::scene_window(Scene& scene)
    {
        ImGui::Begin("Scene Controls", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
        {
            ImGui::SetWindowPos(ImVec2(10, 10), ImGuiCond_Once);
            // ImGui::SetWindowSize(ImVec2(300, 300), ImGuiCond_Once);
            ImGui::SetWindowCollapsed(true, ImGuiCond_Once); 

            camera_component(scene.m_camera);   
            lights_component(*scene.m_lights);
            entities_component(scene.m_entities);
        }   
        ImGui::End();
    }

    void DebugUI::camera_component(Camera& camera)
    {
        ImGui::Text("Camera");
        ImGui::Separator();
        ImGui::DragFloat3("Position", (float*)&camera.m_position); // C cast aaaaaaaaa
        ImGui::Text("Alpha: %f", glm::degrees(camera.m_alpha_rads));
        ImGui::Text("Phi: %f", glm::degrees(camera.m_phi_rads));

        ImGui::SliderFloat("Speed", &camera.m_speed, 0.1f, 100.0f);
        ImGui::SliderFloat("Sensitivity", &camera.m_sensitivity, 0.1f, 10.0f);

        ImGui::Separator();
    }

    void DebugUI::lights_component(std::vector<PointLight>& lights)
    {
        ImGui::Text("Point Lights");
        ImGui::Separator();

        for (auto& light : lights)
        {
            ImGui::PushID(&light);
            auto sliders = {ImGui::DragFloat3("Position", (float*)&light.position),
                ImGui::ColorEdit3("Color", (float*)&light.color),
                ImGui::SliderFloat("Ambient Intensity", &light.ambient_intensity, 0.0f, 2.0f),
                ImGui::SliderFloat("Diffuse Intensity", &light.diffuse_intensity, 0.0f, 2.0f),
                ImGui::SliderFloat("Specular Intensity", &light.specular_intensity, 0.0f, 2.0f)
            };
            
            if (std::any_of(sliders.begin(), sliders.end(), [](bool b) { return b; }))
                light.notify(light);

            ImGui::PopID();
        }
    }

    void DebugUI::entities_component(std::vector<std::unique_ptr<RenderableEntity>>& entities)
    {
        ImGui::Text("Entities");
        ImGui::Separator();
        if (ImGui::TreeNode("Renderable Entities"))
        {
            for (size_t i = 0; i < entities.size(); i++)
            {
                auto& entity = entities[i];
                ImGui::PushID(&entity);

                if (ImGui::TreeNode((void*)(intptr_t)i, "Entity %zu", i))
                {
                    ImGui::DragFloat3("Position", (float*)&entity->transform().position_data);
                    ImGui::DragFloat3("Rotation", (float*)&entity->transform().rotation_data);
                    ImGui::DragFloat3("Scale", (float*)&entity->transform().scale_data);

                    if (entity->m_material)
                    {
                        if (ImGui::TreeNode("Material"))
                        {
                            if (entity->m_material->kind() == Material::Kind::PhongBlinn)
                                phong_blinn_material_component(*std::static_pointer_cast<PhongBlinnMaterial>(entity->m_material));

                            ImGui::TreePop();
                        }
                    }
                    ImGui::TreePop();
                }
                
                ImGui::PopID();
            }
            ImGui::TreePop();
        }
    }

    void DebugUI::phong_blinn_material_component(PhongBlinnMaterial& material)
    {
        ImGui::Text("Phong Blinn Material");
        ImGui::Separator();
        ImGui::ColorEdit3("Ambient Color", (float*)&material.m_ambient_color);
        ImGui::ColorEdit3("Diffuse Color", (float*)&material.m_diffuse_color);
        ImGui::ColorEdit3("Specular Color", (float*)&material.m_specular_color);

        ImGui::SliderFloat("Specular Shininess", &material.m_specular_shininess, 1.0f, 512.0f, "%.2f", ImGuiSliderFlags_Logarithmic);
    }

}
