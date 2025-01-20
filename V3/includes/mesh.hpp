#pragma once

#include "vertex.hpp"

#include "glm/glm.hpp"

#include <iostream>
#include <memory>
#include <vector>


namespace FGE
{
    class Mesh
    {
    public:
        Mesh (const Mesh& other);
        Mesh (std::vector<Vertex> vertices, std::vector<unsigned int> indices);

        Mesh& operator=(const Mesh& other)
        {
            m_vertices = other.m_vertices;
            m_indices = other.m_indices;

            this->m_translation_matrix = other.m_translation_matrix;
            this->m_rotation_matrix = other.m_rotation_matrix;
            this->m_scaling_matrix = other.m_scaling_matrix;

            printf ("Equal operator on %p\n", this);

            return (*this);
        }

        Mesh (Mesh&& other)
        {
            m_vertices = std::move(other.m_vertices);
            m_indices = std::move(other.m_indices);

            this->m_translation_matrix = other.m_translation_matrix;
            this->m_rotation_matrix = other.m_rotation_matrix;
            this->m_scaling_matrix = other.m_scaling_matrix;

            other.m_translation_matrix = { 0 };
            other.m_rotation_matrix = { 0 };
            other.m_scaling_matrix = { 0 };

            printf ("Move Constructor ! on %p\n", this);
        }

        ~Mesh ();

        std::vector<Vertex>& GetVertices ();
        std::vector<unsigned int>& GetIndices ();

        void Translate (glm::vec3 vector);
        void Rotate (glm::vec3 vector);
        void Scale (glm::vec3 vector);

        glm::mat4 GetMatrix ();

    private:
        std::shared_ptr<std::vector<Vertex>> m_vertices;
        std::shared_ptr<std::vector<unsigned int>> m_indices;

        glm::mat4 m_translation_matrix;
        glm::mat4 m_rotation_matrix;
        glm::mat4 m_scaling_matrix;
    };
}