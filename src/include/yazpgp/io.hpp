#pragma once
#include "mesh.hpp"
#include <string>
#include <optional>
#include <memory>
#include "shader.hpp"

namespace yazpgp
{
    namespace io
    {
        std::shared_ptr<Mesh> load_mesh_from_file(const std::string& path);
        std::shared_ptr<Shader> load_shader_from_file(const std::string& vertex_path, const std::string& fragment_path);
        std::optional<std::string> slurp_file(const std::string& path);
    }
} 
