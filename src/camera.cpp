#include "camera.hpp"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <iostream>
namespace yazpgp
{
    constexpr float PI = 3.14159265359f;
    constexpr float DEG_TO_RAD = PI / 180.0f;
    constexpr float epsilon = 0.0001f;

    // Camera::Camera(glm::vec3 position, float alpha_deg, float phi_deg, glm::vec3 up)
    //     : m_position(position), m_up(up), m_alpha_rads(alpha_deg * DEG_TO_RAD), m_phi_rads(phi_deg * DEG_TO_RAD)
    // {
    // }

    Camera::Camera(
        glm::vec3 position,
        float speed,
        float sensitivity,
        float alpha_deg,
        float phi_deg,
        glm::vec3 up
    )
    : m_position(position)
    , m_up(up)
    , m_alpha_rads(alpha_deg * DEG_TO_RAD)
    , m_phi_rads(phi_deg * DEG_TO_RAD)
    , m_speed(speed)
    , m_sensitivity(sensitivity)
    {}


    glm::vec3 Camera::target() const
    {
        return glm::vec3(
            std::sin(m_alpha_rads) * cos(m_phi_rads),
            std::cos(m_alpha_rads),
            std::sin(m_alpha_rads) * sin(m_phi_rads)
        );
    }

    glm::mat4 Camera::view_matrix() const
    {
        return glm::lookAt(m_position, m_position + target(), m_up);
    }


    void Camera::rotate_right(float angle_deg)
    {
        m_phi_rads += angle_deg * DEG_TO_RAD;
        m_phi_rads = std::fmod(m_phi_rads, 2 * PI);
    }

    void Camera::rotate_up(float angle_deg)
    {
        m_alpha_rads -= angle_deg * DEG_TO_RAD;
        m_alpha_rads = std::clamp(m_alpha_rads, epsilon, PI - epsilon);
    }

    void Camera::move_forward(float distance)
    {
        m_position += distance * target();
    }

    void Camera::move_right(float distance)
    {
        m_position += distance * glm::cross(target(), m_up);
    }

    void Camera::move_up(float distance)
    {
        m_position += distance * m_up;
    }

    void Camera::update(const InputManager& input, float delta_time)
    {
        auto original_speed = m_speed;
        if (input.get_key(Key::LSHIFT))
            m_speed *= 2.0f;

        if (input.get_key(Key::W))
            move_forward(m_speed * delta_time);
        if (input.get_key(Key::S))
            move_forward(-m_speed * delta_time);
        if (input.get_key(Key::D))
            move_right(m_speed * delta_time);
        if (input.get_key(Key::A))
            move_right(-m_speed * delta_time);
        if (input.get_key(Key::E))
            move_up(m_speed * delta_time);
        if (input.get_key(Key::Q))
            move_up(-m_speed * delta_time);
    
        m_speed = original_speed;

        m_alpha_rads += input.mouse_delta_y() * m_sensitivity * DEG_TO_RAD;
        m_phi_rads += input.mouse_delta_x() * m_sensitivity * DEG_TO_RAD;
    }
}
