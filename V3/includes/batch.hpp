#pragma once

#include "glad/glad.h"

#include "mesh.hpp"
#include "shader.hpp"
#include "texture_atlas.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"


#include <stdexcept>
#include <string>

namespace FGE
{
    class Batch
    {
    private:
        unsigned int m_vao;
        unsigned int m_vbo;
        unsigned int m_ebo;
        unsigned int m_dibo;
        unsigned int m_matrix_ssbo;
        unsigned int m_tex_coord_ssbo;

        Shader* m_shader = nullptr;
        TextureAtlas* m_atlas = nullptr;

        unsigned int m_index_count = 0;
        unsigned int m_vertex_count = 0;
        std::vector<Mesh*> m_meshes;

        unsigned int m_max_index_count = 0;
        unsigned int m_max_vertex_count = 0;
        unsigned int m_max_mesh_count = 0;

        /**
         * @brief Resize the buffer objects of the batch.
         * 
         * @param nb_mesh The number of mesh to be added.
         * @param nb_vertex The number of vertices to be added.
         * @param nb_index The number of indices to be added.
         */
        void Resize (int mesh_count, int vertex_count, int index_count);
    
    public:
        /**
         * @brief Construct a new FGE::Batch::Batch object
         * 
         * @param shader_key The key corresponding to the desired shader.
         * @param atlas_key The key corresponding to the desired atlas.
         */
        Batch(std::string shader_key, std::string atlas_key);

        /**
         * @brief Destroy the FGE::Batch::Batch object.
         * 
         */
        ~Batch();

        /**
         * @brief Bind all the OpenGL buffer objects of the batch.
         * 
         */
        void BindBuffers ();

        /**
         * @brief Unbind all the OpenGL buffer objects of the batch.
         * 
         */
        void UnbindBuffers ();

        /**
         * @brief Push a mesh inside the batch.
         * 
         * @param mesh A pointer to the mesh to be pushed.
         */
        void PushMesh (Mesh* mesh);

        /**
         * @brief Update the matrix buffer of the batch.
         * 
         * @param matrix_offset An offset inside the buffer corresponding to the starting position of the matrix.
         * @param matrix The matrix to replace the old one.
         */
        void UpdateMatrix (unsigned int matrix_offset, glm::mat4 matrix);

        /**
         * @brief Returns the number of mesh.
         * 
         * @return size_t
         */
        size_t GetMeshCount ();

        /**
         * @brief Returns a pointer to the shader stored.
         * 
         * @return FGE::Shader*
         */
        Shader* GetShader ();

        /**
         * @brief Return the OpenGL object bound to the texture.
         * 
         * @return unsigned int 
         */
        unsigned int GetAtlasObject ();
    };
}