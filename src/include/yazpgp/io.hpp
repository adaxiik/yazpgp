#pragma once
#include <string>
#include <optional>
#include <memory>
#include "shader.hpp"
#include "mesh.hpp"
#include "texture_2d.hpp"
#include "cubemap.hpp"

namespace yazpgp
{
    namespace io
    {
        std::shared_ptr<Mesh> load_mesh_from_file(const std::string& path);
        std::shared_ptr<Shader> load_shader_from_file(const std::string& vertex_path, const std::string& fragment_path);
        std::shared_ptr<Texture2D> load_texture_from_file(const std::string& path);
        std::optional<std::string> slurp_file(const std::string& path);

        /**
         * @brief Loads a cubemap from 6 files
         * 
         * @param paths in order: right, left, top, bottom, front, back
         * @return std::shared_ptr<CubeMap> 
         */
        std::shared_ptr<CubeMap> load_cubemap_from_files(const std::array<std::string, 6>& paths);
    }
} 
