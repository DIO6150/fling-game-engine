#include <iostream>

#include "engine.hpp"
#include "mesh.hpp"
#include "scene.hpp"

FGE::Engine* FGE::Engine::m_instance = nullptr;
std::mutex FGE::Engine::m_mtx;

int main (int argc, char** argv)
{
    std::vector<FGE::Vertex> vertices = {
        (FGE::Vertex) {0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
        (FGE::Vertex) {0.5f, 0.0f, 0.0f, 0.0f, 1.0f},
        (FGE::Vertex) {0.5f, 0.5f, 0.0f, 1.0f, 1.0f}
    };

    std::vector<FGE::Vertex> vertices_2 = {
        (FGE::Vertex) {0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
        (FGE::Vertex) {0.5f, 0.0f, 0.0f, 0.0f, 1.0f},
        (FGE::Vertex) {0.5f, 0.5f, 0.0f, 1.0f, 1.0f},
        (FGE::Vertex) {0.0f, 0.5f, 0.0f, 1.0f, 1.0f}
    };

    std::vector<unsigned int> indices ={
        0, 1, 2
    };

    std::vector<unsigned int> indices_2 ={
        0, 1, 2, 2, 3, 0
    };

    FGE::Engine::GetInstance ();

    FGE::Shader default_shader ("assets/shaders/default.vertex", "assets/shaders/default.fragment");
    FGE::Mesh triangle (vertices, indices);
    FGE::Mesh square (vertices_2, indices_2);
    


    FGE::Engine::RegisterGameObject<FGE::Shader> ("k_shader_default", default_shader);
    FGE::Engine::RegisterGameObject<FGE::Mesh> ("k_mesh_triangle", triangle);
    FGE::Engine::RegisterGameObject<FGE::Mesh> ("k_mesh_square", square);

    FGE::Engine::UploadMesh ("k_mesh_triangle", {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.5, 0.5});
    FGE::Engine::UploadMesh ("k_mesh_triangle", {0.0, -0.5, 0.0});
    FGE::Engine::UploadMesh ("k_mesh_square", {-0.5, -0.5, 0.0});
    FGE::Engine::UploadMesh ("k_mesh_square", {-0.5, -1.0, 0.0}, {0.0, 0.0, 45.0});

    FGE::Engine::LoadBatches ("k_shader_default");

    while (!FGE::Engine::ShouldWindowClose())
    {
        glfwPollEvents ();
        FGE::Engine::Render ();
    }

    return (1);
}