#include "io.hpp"
#include "logger.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

namespace yazpgp
{
    namespace io
    {
        std::optional<Mesh> load_mesh_from_file(const std::string& path)
        {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);
            if (not scene)
            {
                YAZPGP_LOG_ERROR("Failed to load mesh from file: %s", path.c_str());
                YAZPGP_LOG_ERROR("Error: %s", importer.GetErrorString());
                return std::nullopt;
            }
            if (scene->mNumMeshes == 0)
            {
                YAZPGP_LOG_ERROR("No meshes found in file: %s", path.c_str());
                return std::nullopt;
            }

            std::vector<float> vertex_data;
            size_t estimated_vertex_data_size = 0;
            for (size_t i = 0; i < scene->mNumMeshes; i++)
                estimated_vertex_data_size += scene->mMeshes[i]->mNumVertices * 6;

            vertex_data.reserve(estimated_vertex_data_size);

            for (size_t i = 0; i < scene->mNumMeshes; i++)
            {
                const aiMesh* mesh = scene->mMeshes[i];
                for (size_t j = 0; j < mesh->mNumVertices; j++)
                {
                    vertex_data.push_back(mesh->mVertices[j].x);
                    vertex_data.push_back(mesh->mVertices[j].y);
                    vertex_data.push_back(mesh->mVertices[j].z);
                    vertex_data.push_back(mesh->mNormals[j].x);
                    vertex_data.push_back(mesh->mNormals[j].y);
                    vertex_data.push_back(mesh->mNormals[j].z);
                }
            }

            return Mesh(
                vertex_data.data(),
                vertex_data.size() * sizeof(float),
                VertexAttributeLayout({
                    {.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE},
                    {.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE}
                }));
        }
    }
}