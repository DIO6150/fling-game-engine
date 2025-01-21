#pragma once

#include "texture_atlas.hpp"

#include "vertex.hpp"

#include "glm/glm.hpp"

#include <iostream>
#include <memory>
#include <vector>


namespace FGE
{
    class Batch;

    class Mesh
    {
        struct MeshMeta
        {
            Batch* m_batch;
            unsigned int m_position;
            unsigned int m_matrix_offset;
        };

    public:
        /**
         * @brief Construct a new Mesh object
         * 
         * @param vertices An array of vertices to construct the mesh.
         * @param indices An array of indices to construct the mesh.
         * @param origin A 4x4 matrix giving, relative to the mesh, a point of origin.
         */
        Mesh (std::vector<Vertex> vertices, std::vector<unsigned int> indices, glm::mat4 origin = glm::mat4 (1.0f));

        /**
         * @brief Destroy the Mesh object
         * 
         */
        ~Mesh ();

        /**
         * @brief Construct a new Mesh object (Copy Constructor)
         * 
         * @param other 
         */
        Mesh (const Mesh& other);

        /**
         * @brief Construct a new Mesh object (Move constructor)
         * 
         * @param other 
         */
        Mesh (Mesh&& other);

        /**
         * @brief Assignement Operator
         * 
         * @param other 
         * @return Mesh& 
         */
        Mesh& operator=(const Mesh& other);

        /**
         * @brief Get an array of vertices.
         * 
         * @return std::vector<Vertex>& 
         */
        std::vector<Vertex>& GetVertices ();

        /**
         * @brief Get an array of indices.
         * 
         * @return std::vector<unsigned int>& 
         */
        std::vector<unsigned int>& GetIndices ();

        /**
         * @brief Translate the mesh relative to its own matrix.
         * 
         * @param vector 
         */
        void Translate (glm::vec3 vector);

        /**
         * @brief Rotate the mesh relative to its own matrix.
         * 
         * @param vector 
         */
        void Rotate (glm::vec3 vector);

        /**
         * @brief Displace the point of origin of the mesh.
         * 
         * @param vector A 3-dimensional vector with an angle as each of his components.
         */
        void Origin (glm::vec3 vector);

        /**
         * @brief Scale the mesh relative to its own matrix.
         * 
         * @param vector 
         */
        void Scale (glm::vec3 vector);

        /**
         * @brief Set the translation matrix.
         * 
         * @param vector 
         */
        void SetTranslate (glm::vec3 vector);

        /**
         * @brief Set the rotation matrix.
         * 
         * @param vector A 3-dimensional vector with an angle as each of his components.
         */
        void SetRotate (glm::vec3 vector);

        /**
         * @brief Set the origin of the mesh (relative to himself).
         * 
         * @param vector 
         */
        void SetOrigin (glm::vec3 vector);

        /**
         * @brief Set the scale of the mesh.
         * 
         * @param vector 
         */
        void SetScale (glm::vec3 vector);

        /**
         * @brief Returns a matrix product of Translation * Rotation * Origin * Scale
         * 
         * @return glm::mat4 
         */
        glm::mat4 GetMatrix ();

        /**
         * @brief Set the texture wrapping around the mesh
         * 
         * @param atlas_key The key of the atlas in which the texture is.
         * @param texture_key The key of the texture.
         */
        void SetTexture (std::string atlas_key, std::string texture_key);

        /**
         * @brief Returns the quad that represents space in a texture atlas.
         * 
         * @return TextureQuad* 
         */
        TextureQuad* GetQuad ();

        /**
         * @brief Return a pointer to the batch in which the Mesh is. (nullptr if the mesh is not recorded in any batch).
         * 
         * @return Batch* 
         */
        Batch* GetBatch ();

        /**
         * @brief Get the position of the mesh inside an OpenGL buffer in a batch object.
         * 
         * @return unsigned int 
         */
        unsigned int GetPositionOffset ();

        /**
         * @brief Get the position of the matrix inside an OpenGL buffer in a batch object.
         * 
         * @return unsigned int 
         */
        unsigned int GetMatrixOffset ();

        /**
         * @brief Set the Batch in which the mesh should be. (Doesn't sync the content of the batch tho).
         * 
         * @param batch 
         */
        void SetBatch (Batch* batch);

        /**
         * @brief Set the position of the mesh inside an OpenGL buffer in a batch object.
         * 
         * @param position_offset 
         */
        void SetPositionOffset (unsigned int position_offset);

        /**
         * @brief Set the position  of the matrix inside an OpenGL buffer in a batch object.
         * 
         * @param matrix_offset 
         */
        void SetMatrixOffset (unsigned int matrix_offset);

    private:
        std::shared_ptr<std::vector<Vertex>> m_vertices;
        std::shared_ptr<std::vector<unsigned int>> m_indices;

        glm::mat4 m_translation_matrix;
        glm::mat4 m_rotation_matrix;
        glm::mat4 m_origin_matrix;
        glm::mat4 m_scaling_matrix;

        TextureQuad* m_quad = nullptr;

        // cached renderer data
        MeshMeta m_mesh_meta;
    };
}