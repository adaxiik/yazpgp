#include "io.hpp"
#include "logger.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <fstream>
namespace yazpgp
{
    namespace io
    {
        std::shared_ptr<Mesh> load_mesh_from_file(const std::string& path)
        {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);
            if (not scene)
            {
                YAZPGP_LOG_ERROR("Failed to load mesh from file: %s", path.c_str());
                YAZPGP_LOG_ERROR("Error: %s", importer.GetErrorString());
                return nullptr;
            }
            if (scene->mNumMeshes == 0)
            {
                YAZPGP_LOG_ERROR("No meshes found in file: %s", path.c_str());
                return nullptr;
            }

            std::vector<Vertex> vertex_data;
            size_t estimated_vertex_data_size = 0;
            for (size_t i = 0; i < scene->mNumMeshes; i++)
                estimated_vertex_data_size += scene->mMeshes[i]->mNumVertices;
            
            vertex_data.reserve(estimated_vertex_data_size);

            for (size_t i = 0; i < scene->mNumMeshes; i++)
            {
                const aiMesh* mesh = scene->mMeshes[i];
                for (size_t j = 0; j < mesh->mNumVertices; j++)
                {
                    vertex_data.push_back({
                        .x = mesh->mVertices[j].x,
                        .y = mesh->mVertices[j].y,
                        .z = mesh->mVertices[j].z,
                        .nx = mesh->mNormals[j].x,
                        .ny = mesh->mNormals[j].y,
                        .nz = mesh->mNormals[j].z
                    });
                }
            }

            std::vector<uint32_t> indices;
            size_t estimated_index_data_size = 0;
            for (size_t i = 0; i < scene->mNumMeshes; i++)
                estimated_index_data_size += scene->mMeshes[i]->mNumFaces * 3;
            
            indices.reserve(estimated_index_data_size);

            for (size_t i = 0; i < scene->mNumMeshes; i++)
            {
                const aiMesh* mesh = scene->mMeshes[i];
                for (size_t j = 0; j < mesh->mNumFaces; j++)
                {
                    indices.push_back(mesh->mFaces[j].mIndices[2]);
                    indices.push_back(mesh->mFaces[j].mIndices[1]);
                    indices.push_back(mesh->mFaces[j].mIndices[0]);
                }
            }
            
            return std::make_shared<Mesh>(vertex_data, indices, VertexAttributeLayout({
                {.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE},
                {.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE}
            }));
        }
    
        std::shared_ptr<Shader> load_shader_from_file(const std::string& vertex_path, const std::string& fragment_path)
        {
            auto vertex_source = slurp_file(vertex_path);
            if (not vertex_source.has_value())
                return nullptr;

            auto fragment_source = slurp_file(fragment_path);
            if (not fragment_source.has_value())
                return nullptr;

            return Shader::create_shader(vertex_source.value(), fragment_source.value());
        }


        std::optional<std::string> slurp_file(const std::string& path)
        {
            std::ifstream file(path);
            if (file.fail())
            {
                YAZPGP_LOG_ERROR("Failed to open file: %s", path.c_str());
                return std::nullopt;
            }


            // probably very unsafe and scary
            std::string contents;
            file.seekg(0, std::ios::end);
            contents.resize(file.tellg());
            file.seekg(0, std::ios::beg);
            file.read(&contents[0], contents.size());

            return contents;
        }
    }
}