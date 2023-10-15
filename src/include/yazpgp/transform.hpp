#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <variant>
#include <vector>

namespace yazpgp
{
    struct Transform
    {
        glm::vec3 position_data = {0, 0, 0};
        glm::vec3 rotation_data = {0, 0, 0};
        glm::vec3 scale_data = {1, 1, 1};

        glm::mat4 model_matrix() const
        {
            glm::mat4 rotation_matrix = glm::toMat4(glm::quat(glm::radians(rotation_data)));
            return glm::translate(glm::mat4(1.0f), position_data) * rotation_matrix * glm::scale(glm::mat4(1.0f), scale_data);
        }

        Transform copy() const
        {
            return Transform(*this);
        }
        
        Transform() = default;
        Transform(const Transform &) = default;
        static Transform default_transform()
        {
            return Transform();
        }

        Transform(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale) : position_data(position), rotation_data(rotation), scale_data(scale) {}
        Transform(const glm::vec3 &position, const glm::vec3 &rotation) : position_data(position), rotation_data(rotation) {}
        Transform(const glm::vec3 &position) : position_data(position) {}

        Transform &translate(const glm::vec3 &translation)
        {
            position_data += translation;
            return *this;
        }

        Transform &rotate(const glm::vec3 &rotation)
        {
            rotation_data += rotation;
            return *this;
        }

        Transform &scale(const glm::vec3 &scale)
        {
            scale_data *= scale;
            return *this;
        }

        Transform translate_copy(const glm::vec3 &translation) const
        {
            return Transform(*this).translate(translation);
        }

        Transform rotate_copy(const glm::vec3 &rotation) const
        {
            return Transform(*this).rotate(rotation);
        }

        Transform scale_copy(const glm::vec3 &scale) const
        {
            return Transform(*this).scale(scale);
        }
    

    // class Transform
    // {
    //     glm::mat4 m_matrix;

    // public:
    //     const glm::mat4 &model_matrix() const
    //     {
    //         return m_matrix;
    //     }

    //     Transform copy() const
    //     {
    //         return Transform(*this);
    //     }

    //     Transform() : m_matrix(glm::mat4(1.0f)) {}

    //     Transform(const glm::mat4 &matrix) : m_matrix(matrix) {}

    //     Transform &translate(const glm::vec3 &translation)
    //     {
    //         m_matrix = glm::translate(m_matrix, translation);
    //         return *this;
    //     }

    //     Transform &rotate(const glm::vec3 &rotation)
    //     {
    //         m_matrix = glm::rotate(m_matrix, glm::radians(rotation.x), {1, 0, 0});
    //         m_matrix = glm::rotate(m_matrix, glm::radians(rotation.y), {0, 1, 0});
    //         m_matrix = glm::rotate(m_matrix, glm::radians(rotation.z), {0, 0, 1});
    //         return *this;
    //     }

    //     Transform &scale(const glm::vec3 &scale)
    //     {
    //         m_matrix = glm::scale(m_matrix, scale);
    //         return *this;
    //     }

    //     Transform translate_copy(const glm::vec3 &translation) const
    //     {
    //         return Transform(*this).translate(translation);
    //     }

    //     Transform rotate_copy(const glm::vec3 &rotation) const
    //     {
    //         return Transform(*this).rotate(rotation);
    //     }

    //     Transform scale_copy(const glm::vec3 &scale) const
    //     {
    //         return Transform(*this).scale(scale);
    //     }

    //     static Transform default_transform()
    //     {
    //         return Transform();
    //     }


    //     // usage looks like this:
    //     // entity->transform() = Transform::Compositor::Composite({
    //     //     Transform::Compositor::Rotate({0, 1, 0}),
    //     //     Transform::Compositor::Composite({
    //     //         Transform::Compositor::Translate({0, 0, -0.5}),
    //     //         Transform::Compositor::Rotate({0, 1, 0})
    //     //     })
    //     // })(entity->transform());

        struct Compositor
        {
            struct TranslateData
            {
                glm::vec3 translation;
            };
            struct RotateData
            {
                glm::vec3 rotation;
            };
            struct ScaleData
            {
                glm::vec3 scale;
            };
            struct CompositeData
            {
                std::vector<Compositor> compositor;
            };

            using CompositorVariant = std::variant<TranslateData, RotateData, ScaleData, CompositeData>;
            CompositorVariant compositor;

            Compositor(const TranslateData &translate) : compositor(translate) {}
            Compositor(const RotateData &rotate) : compositor(rotate) {}
            Compositor(const ScaleData &scale) : compositor(scale) {}
            Compositor(const CompositeData &composite) : compositor(composite) {}

            Transform operator()(const Transform &transform) const
            {
                auto c = transform.copy();
                return std::visit(CompositorVisitor{c}, compositor);
            }

            Transform operator()(Transform &&transform) const
            {
                return std::visit(CompositorVisitor{transform}, compositor);
            }

            Transform compose(const Transform &transform) const
            {
                return (*this)(transform);
            }

            struct CompositorVisitor
            {
                Transform &transform;
                Transform operator()(const TranslateData &translate) const
                {
                    return transform.translate(translate.translation);
                }
                Transform operator()(const RotateData &rotate) const
                {
                    return transform.rotate(rotate.rotation);
                }
                Transform operator()(const ScaleData &scale) const
                {
                    return transform.scale(scale.scale);
                }
                Transform operator()(const CompositeData &composite) const
                {
                    Transform result = transform;
                    for (const auto &compositor : composite.compositor)
                        result = compositor(result);
                    return result;
                }
            };

            static Compositor Translate(const glm::vec3 &translation)
            {
                return Compositor(TranslateData{translation});
            }

            static Compositor Rotate(const glm::vec3 &rotation)
            {
                return Compositor(RotateData{rotation});
            }

            static Compositor Scale(const glm::vec3 &scale)
            {
                return Compositor(ScaleData{scale});
            }

            static Compositor Composite(const std::vector<Compositor> &compositor)
            {
                return Compositor(CompositeData{compositor});
            }

            static Compositor Composite(std::vector<Compositor> &&compositor)
            {
                return Compositor(CompositeData{std::move(compositor)});
            }

            static Compositor Composite(std::initializer_list<Compositor> compositor)
            {
                return Compositor(CompositeData{compositor});
            }
        };
    };
}