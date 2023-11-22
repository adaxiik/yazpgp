#pragma once
#include <glm/glm.hpp>
#include <array>

namespace yazpgp
{

    template<size_t N>
    class BezierCurve
    {
        std::array<glm::vec3, N> m_points;

        static glm::vec3 lerp(const glm::vec3& a, const glm::vec3& b, float t)
        {
            return a + t * (b - a);
        }

    public:
        glm::vec3 operator()(float t) const
        {
            if constexpr (N == 2)
            {
                return lerp(m_points[0], m_points[1], t);
            }
            else {
                std::array<glm::vec3, N - 1> new_points;
                for (size_t i = 0; i < N - 1; i++)
                    new_points[i] = lerp(m_points[i], m_points[i + 1], t);

                return BezierCurve<N - 1>(new_points)(t);
            }
        }

        BezierCurve(const std::array<glm::vec3, N>& points)
            : m_points(points)
        {}

    };
}