#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "bezier_curve.hpp"

namespace yazpgp
{
    template<size_t N>
    class BezierList
    {
        std::vector<BezierCurve<N>> m_curves;

    public:
        glm::vec3 operator()(float t) const
        {
            if (m_curves.empty())
                return glm::vec3(0.0f);

            float curve_t = t * m_curves.size();
            size_t curve_index = static_cast<size_t>(curve_t);
            curve_t -= curve_index;

            if (curve_index >= m_curves.size())
                return m_curves.back()(1.0f);

            return m_curves[curve_index](curve_t);

        }

        BezierList(const std::vector<BezierCurve<N>>& curves)
            : m_curves(curves)
        {}

        BezierList()
            : m_curves()
        {}

        BezierList& add_curve(const BezierCurve<N>& curve)
        {
            m_curves.push_back(curve);
            return *this;
        }
    };
}