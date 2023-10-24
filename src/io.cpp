#include "io.hpp"
#include "logger.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <fstream>
#include <SDL2/SDL_image.h>


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

            
            bool has_uvs = scene->mMeshes[0]->HasTextureCoords(0);
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
                        .nz = mesh->mNormals[j].z,
                        .u = has_uvs ? mesh->mTextureCoords[0][j].x : 0.0f,
                        .v = has_uvs ? mesh->mTextureCoords[0][j].y : 0.0f
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
                    for (size_t k = 0; k < mesh->mFaces[j].mNumIndices; k++)
                        indices.push_back(mesh->mFaces[j].mIndices[k]);
                }
            }
            
            return std::make_shared<Mesh>(vertex_data, indices, VertexAttributeLayout({
                {.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE},
                {.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE},
                {.size = 2, .type = GL_FLOAT, .normalized = GL_FALSE}
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

        std::shared_ptr<Texture2D> load_texture_from_file(const std::string& path)
        {
            SDL_Surface* surface = IMG_Load(path.c_str());
            if (not surface)
            {
                YAZPGP_LOG_ERROR("Failed to load texture from file: %s", path.c_str());
                YAZPGP_LOG_ERROR("Error: %s", IMG_GetError());
                return nullptr;
            }

            // engineers in SDL couldn't add the most used function in image processing with opengl :))
            for (int y = 0; y < surface->h / 2; y++)
            {
                char* top_row = static_cast<char*>(surface->pixels) + y * surface->pitch;
                char* bottom_row = static_cast<char*>(surface->pixels) + (surface->h - y - 1) * surface->pitch;
                for (int x = 0; x < surface->w * surface->format->BytesPerPixel; x++)
                    std::swap(top_row[x], bottom_row[x]);
            }


            auto texture =  std::make_shared<Texture2D>(
                static_cast<const char*>(surface->pixels),
                surface->w,
                surface->h,
                surface->format->BytesPerPixel
            );

            SDL_FreeSurface(surface);
            return texture;
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

        std::shared_ptr<CubeMap> load_cubemap_from_files(const std::array<std::string, 6>& paths)
        {
            SDL_Surface* surfaces[6];
            for (size_t i = 0; i < 6; i++)
            {
                surfaces[i] = IMG_Load(paths[i].c_str());
                if (not surfaces[i])
                {
                    YAZPGP_LOG_ERROR("Failed to load cubemap from file: %s", paths[i].c_str());
                    YAZPGP_LOG_ERROR("Error: %s", IMG_GetError());
                    return nullptr;
                }
            }

            auto cubemap = std::make_shared<CubeMap>(std::array<CubeMap::CubeMapDataPart, 6>({
                CubeMap::CubeMapDataPart{
                    .bytes = static_cast<const char*>(surfaces[0]->pixels),
                    .width = surfaces[0]->w,
                    .height = surfaces[0]->h,
                    .channels = surfaces[0]->format->BytesPerPixel
                },
                {
                    .bytes = static_cast<const char*>(surfaces[1]->pixels),
                    .width = surfaces[1]->w,
                    .height = surfaces[1]->h,
                    .channels = surfaces[1]->format->BytesPerPixel
                },
                {
                    .bytes = static_cast<const char*>(surfaces[2]->pixels),
                    .width = surfaces[2]->w,
                    .height = surfaces[2]->h,
                    .channels = surfaces[2]->format->BytesPerPixel
                },
                {
                    .bytes = static_cast<const char*>(surfaces[3]->pixels),
                    .width = surfaces[3]->w,
                    .height = surfaces[3]->h,
                    .channels = surfaces[3]->format->BytesPerPixel
                },
                {
                    .bytes = static_cast<const char*>(surfaces[4]->pixels),
                    .width = surfaces[4]->w,
                    .height = surfaces[4]->h,
                    .channels = surfaces[4]->format->BytesPerPixel
                },
                {
                    .bytes = static_cast<const char*>(surfaces[5]->pixels),
                    .width = surfaces[5]->w,
                    .height = surfaces[5]->h,
                    .channels = surfaces[5]->format->BytesPerPixel
                }
            }));

            for (size_t i = 0; i < 6; i++)
                SDL_FreeSurface(surfaces[i]);

            return cubemap;
        }
    }
}