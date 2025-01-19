#pragma once

#include "vertex.hpp"

#include "glm/glm.hpp"

#include <vector>

namespace FGE
{
    class Mesh
    {
    public:
        Mesh();
        ~Mesh();

        std::vector<Vertex>& GetVertices ();
        std::vector<unsigned int>& GetIndices ();

        glm::mat4& GetMatrix ();

    private:
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;

        glm::mat4 m_matrix;
    };
}