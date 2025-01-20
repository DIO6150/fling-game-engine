#pragma once

#include <string>
#include <vector>

#include "mesh.hpp"

namespace FGE
{
    class Scene
    {
        std::vector<Mesh> m_meshes;

        // TODO: Add collidables
        std::vector<void*> m_collidables;
    public:
        Scene ();
        ~Scene ();

        void UploadMesh (std::string key, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scaling);
        std::vector<Mesh> GetMeshes () { return m_meshes; };
    };
}