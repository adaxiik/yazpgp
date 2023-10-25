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


        // usage looks like this:
        // entity->transform() = Transform::Compositor::Composite({
        //     Transform::Compositor::Rotate({0, 1, 0}),
        //     Transform::Compositor::Composite({
        //         Transform::Compositor::Translate({0, 0, -0.5}),
        //         Transform::Compositor::Rotate({0, 1, 0})
        //     })
        // })(entity->transform());

        struct Mat4Compositor
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

            using Mat4Data = glm::mat4;
            
            struct CompositeData
            {
                std::vector<Mat4Compositor> compositor;
            };

            using CompositorVariant = std::variant<TranslateData, RotateData, ScaleData, CompositeData, Mat4Data>;
            CompositorVariant compositor;

            Mat4Compositor(const TranslateData &translate) : compositor(translate) {}
            Mat4Compositor(const RotateData &rotate) : compositor(rotate) {}
            Mat4Compositor(const ScaleData &scale) : compositor(scale) {}
            Mat4Compositor(const CompositeData &composite) : compositor(composite) {}
            Mat4Compositor(const Mat4Data &mat4) : compositor(mat4) {}

            glm::mat4 operator()(const glm::mat4 &transform = glm::mat4(1.0f)) const
            {
                // auto c = transform.copy();
                // return std::visit(CompositorVisitor{c}, compositor);
                auto c = transform;
                return std::visit(CompositorVisitor{c}, compositor);
            }

            glm::mat4 operator()(glm::mat4 &&transform) const
            {
                return std::visit(CompositorVisitor{transform}, compositor);
            }

            glm::mat4 compose(const glm::mat4 &transform = glm::mat4(1.0f)) const
            {
                return (*this)(transform);
            }
            struct CompositorVisitor
            {
                glm::mat4 &transform;
                glm::mat4 operator()(const TranslateData &translate) const
                {
                    // return transform.translate(translate.translation);
                    return glm::translate(transform, translate.translation);
                }
                glm::mat4 operator()(const RotateData &rotate) const
                {
                    // return transform.rotate(rotate.rotation);
                    return glm::rotate(transform, glm::radians(rotate.rotation.x), {1, 0, 0}) *
                           glm::rotate(transform, glm::radians(rotate.rotation.y), {0, 1, 0}) *
                           glm::rotate(transform, glm::radians(rotate.rotation.z), {0, 0, 1});
                }
                glm::mat4 operator()(const ScaleData &scale) const
                {
                    // return transform.scale(scale.scale);
                    return glm::scale(transform, scale.scale);
                }

                glm::mat4 operator()(const Mat4Data &mat4) const
                {
                    return mat4 * transform;
                }

                glm::mat4 operator()(const CompositeData &composite) const
                {
                    glm::mat4 result = transform;
                    for (const auto &compositor : composite.compositor)
                        result = compositor(result);
                    return result;
                }
            };

            static Mat4Compositor Translate(const glm::vec3 &translation)
            {
                return Mat4Compositor(TranslateData{translation});
            }

            static Mat4Compositor Rotate(const glm::vec3 &rotation)
            {
                return Mat4Compositor(RotateData{rotation});
            }

            static Mat4Compositor Scale(const glm::vec3 &scale)
            {
                return Mat4Compositor(ScaleData{scale});
            }

            static Mat4Compositor Composite(const std::vector<Mat4Compositor> &compositor)
            {
                return Mat4Compositor(CompositeData{compositor});
            }

            static Mat4Compositor Composite(std::vector<Mat4Compositor> &&compositor)
            {
                return Mat4Compositor(CompositeData{std::move(compositor)});
            }

            static Mat4Compositor Composite(std::initializer_list<Mat4Compositor> compositor)
            {
                return Mat4Compositor(CompositeData{compositor});
            }
        };
    };
}