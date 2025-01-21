#include "scene.hpp"

#include "engine.hpp"

#include <iostream>

#include "mesh.hpp"

FGE::Scene::Scene ()
{

}

FGE::Scene::~Scene ()
{

}

FGE::Mesh* FGE::Scene::UploadMesh (std::string key, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scaling)
{
    Engine* engine = Engine::GetInstance ();

    Mesh* mesh_base = engine->FindGameObject<Mesh> (key);

    if (mesh_base)
    {
        m_meshes.push_back (Mesh (*mesh_base));
        m_meshes.back ().Translate (translation);
        m_meshes.back ().Rotate (rotation);
        m_meshes.back ().Scale (scaling);
    }

    return (&m_meshes.back());
}

std::vector<FGE::Mesh>& FGE::Scene::GetMeshes ()
{
    return m_meshes;
};