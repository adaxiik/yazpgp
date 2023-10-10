#pragma once
#include <glm/glm.hpp>

#include "input_manager.hpp"
#include "event_source.hpp"
namespace yazpgp
{
    class Camera
    {
        glm::mat4 m_view_matrix;
        glm::vec3 m_position;
        const glm::vec3 m_up;
        
        float m_alpha_rads;
        float m_phi_rads;
        float m_speed;
        float m_sensitivity;

        glm::vec3 target() const;
        void rotate_up(float angle_deg);
        void rotate_right(float angle_deg);
        void move_forward(float distance);
        void move_right(float distance);
        void move_up(float distance);

        glm::mat4 compute_view_matrix() const;

    public:
        Camera(
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            float speed = 5.f,
            float sensitivity = 0.2f,
            float alpha_deg = 90.0f,
            float phi_deg = 0.0f,
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)
        );
        const glm::mat4& view_matrix() const;
        void update(const InputManager& input, float delta_time);
    };
}