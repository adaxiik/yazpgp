#pragma once
#include "mesh.hpp"
#include <string>
#include <optional>
namespace yazpgp
{
    namespace io
    {
        std::optional<Mesh> load_mesh_from_file(const std::string& path);
    }
} 
