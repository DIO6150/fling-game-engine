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
        /**
         * @brief Construct a new Scene
         * 
         */
        Scene ();

        /**
         * @brief Destroy the Scene
         * 
         */
        ~Scene ();

        /**
         * @brief Upload a mesh to the scene, making it eligible to be drawn on the screen.
         * 
         * @param key Key of the mesh to be uploaded.
         * @param translation A 4x4 Matrix representing the translation of the mesh relative to itself
         * @param rotation A 4x4 Matrix representing the rotation of the mesh relative to itself
         * @param scaling A 4x4 Matrix representing the scale of the mesh relative to itself
         * @return Mesh* 
         */
        Mesh* UploadMesh (std::string key, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scaling);

        /**
         * @brief Returns the array of meshes stored in the scene
         * 
         * @return std::vector<Mesh>& 
         */
        std::vector<Mesh>& GetMeshes ();
    };
}