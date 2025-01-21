#include <iostream>

#include "engine.hpp"
#include "mesh.hpp"
#include "scene.hpp"
#include "texture.hpp"
#include "texture_atlas.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

int main (int argc, char** argv)
{
    std::vector<FGE::Vertex> vertices = {
        (FGE::Vertex) {0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
        (FGE::Vertex) {0.5f, 0.0f, 0.0f, 1.0f, 0.0f},
        (FGE::Vertex) {0.5f, 0.5f, 0.0f, 1.0f, 1.0f}
    };

    std::vector<FGE::Vertex> vertices_2 = {
        (FGE::Vertex) {0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
        (FGE::Vertex) {0.5f, 0.0f, 0.0f, 1.0f, 0.0f},
        (FGE::Vertex) {0.5f, 0.5f, 0.0f, 1.0f, 1.0f},
        (FGE::Vertex) {0.0f, 0.5f, 0.0f, 0.0f, 1.0f}
    };

    std::vector<unsigned int> indices ={
        0, 1, 2
    };

    std::vector<unsigned int> indices_2 ={
        0, 1, 2, 2, 3, 0
    };

    glm::mat4 origin = glm::translate (glm::mat4 (1.0f), glm::vec3 (-.25f, -.25f, 0.0f));

    FGE::Engine::GetInstance ();

    FGE::Shader default_shader ("assets/shaders/default.vertex", "assets/shaders/default.fragment");
    FGE::Mesh triangle_base (vertices, indices);
    FGE::Mesh square_base (vertices_2, indices_2, origin);
    FGE::TextureAtlas atlas;
    FGE::Texture tex ("assets/textures/peak.png");
    FGE::Texture tex2 ("assets/textures/salvation.jpeg");

    FGE::Engine::RegisterGameObject<std::string> ("k_name_magic_wand", "Magic Wand");

    FGE::Engine::RegisterGameObject<FGE::Shader> ("k_shader_default", default_shader);
    FGE::Engine::RegisterGameObject<FGE::Mesh> ("k_mesh_triangle", triangle_base);
    FGE::Engine::RegisterGameObject<FGE::Mesh> ("k_mesh_square", square_base);
    FGE::Engine::RegisterGameObject<FGE::TextureAtlas> ("k_atlas", atlas);
    FGE::Engine::RegisterGameObject<FGE::Texture> ("k_peak_tex", tex);
    FGE::Engine::RegisterGameObject<FGE::Texture> ("k_salvation_tex", tex2);

    FGE::Engine::CreateBatch ("k_shader_default", "k_atlas");

    FGE::Engine::UploadMesh ("k_mesh_triangle", {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.5, 0.5});
    FGE::Engine::UploadMesh ("k_mesh_triangle", {0.0, -0.5, 0.0});
    FGE::Mesh* standing_square = FGE::Engine::UploadMesh ("k_mesh_square", {-0.5, -0.5, 0.0});
    FGE::Mesh* rotating_square = FGE::Engine::UploadMesh ("k_mesh_square", {-0.5, -1.0, 0.0}, {0.0, 0.0, 45.0});

    standing_square->SetTexture ("k_atlas", "k_salvation_tex");
    rotating_square->SetTexture ("k_atlas", "k_peak_tex");


    FGE::Engine::LoadBatches ("k_shader_default");

    float x = -1.0;
    int left = 1;

    while (!FGE::Engine::ShouldWindowClose())
    {
        glfwPollEvents ();
        FGE::Engine::RotateMesh (rotating_square, {0.0, 0.0, 1.0});

        if (x < -1) left = -left;
        else if (x > 1) left = -left;

        x += left * 0.02;

        // FGE::Engine::TranslateMesh (rotating_square, {x, 0.0, 0.0}, false);
        // std::cout << glm::to_string (rotating_square->GetMatrix ()) << std::endl;

        FGE::Engine::Render ();
    }

    return (1);
}