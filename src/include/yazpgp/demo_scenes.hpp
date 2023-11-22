#pragma once
#include "scene.hpp"
#include "asset_storage.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "window.hpp"

namespace yazpgp::DemoScenes
{
    Scene phong_four_balls(const AssetStorage<Mesh>& meshes, const AssetStorage<Shader>& shaders);
    Scene solar_system(const AssetStorage<Mesh>& meshes, const AssetStorage<Shader>& shaders, const Window& window);
    Scene ball_between_light_and_camera(const AssetStorage<Mesh>& meshes, const AssetStorage<Shader>& shaders);
    Scene squish_test(const AssetStorage<Mesh>& meshes, const AssetStorage<Shader>& shaders, const AssetStorage<Texture>& textures);
    Scene forest(const AssetStorage<Mesh>& meshes, const AssetStorage<Shader>& shaders, const AssetStorage<Texture>& textures);
    Scene normal_mapping(const AssetStorage<Mesh>& meshes, const AssetStorage<Shader>& shaders, const AssetStorage<Texture>& textures);
    Scene shell_texturing(const AssetStorage<Mesh>& meshes, const AssetStorage<Shader>& shaders, const AssetStorage<Texture>& textures);
    Scene terrain(const AssetStorage<Mesh>& meshes, const AssetStorage<Shader>& shaders, const AssetStorage<Texture>& textures);
    Scene bezier_curve(const AssetStorage<Mesh>& meshes, const AssetStorage<Shader>& shaders);
}