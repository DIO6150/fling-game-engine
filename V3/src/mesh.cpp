#include "mesh.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>


FGE::Mesh::Mesh (const Mesh &other)
{
    this->m_vertices = other.m_vertices;
    this->m_indices = other.m_indices;

    this->m_translation_matrix = other.m_translation_matrix;
    this->m_rotation_matrix = other.m_rotation_matrix;
    this->m_scaling_matrix = other.m_scaling_matrix;

    std::cout << "Copy constructor on " << this << std::endl;
}

FGE::Mesh::Mesh (std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
    this->m_vertices = std::make_shared<std::vector<Vertex>> (vertices);
    this->m_indices = std::make_shared<std::vector<unsigned int>> (indices);

    this->m_translation_matrix = glm::mat4 (1.0f);
    this->m_rotation_matrix = glm::mat4 (1.0f);
    this->m_scaling_matrix = glm::mat4 (1.0f);
}

FGE::Mesh::~Mesh ()
{
    printf ("Destructor invoked ! on %p\n", this);
}

std::vector<FGE::Vertex>& FGE::Mesh::GetVertices ()
{
    return (*m_vertices.get());
}

std::vector<unsigned int>& FGE::Mesh::GetIndices ()
{
    return (*m_indices.get());
}

void FGE::Mesh::Translate(glm::vec3 vector)
{
    m_translation_matrix = glm::translate (m_translation_matrix, vector);
}

void FGE::Mesh::Rotate(glm::vec3 vector)
{
    m_rotation_matrix = glm::rotate (m_rotation_matrix, glm::radians (vector.x), {1.0, 0.0, 0.0});
    m_rotation_matrix = glm::rotate (m_rotation_matrix, glm::radians (vector.y), {0.0, 1.0, 0.0});
    m_rotation_matrix = glm::rotate (m_rotation_matrix, glm::radians (vector.z), {0.0, 0.0, 1.0});
}

void FGE::Mesh::Scale(glm::vec3 vector)
{
    m_scaling_matrix = glm::scale (m_scaling_matrix, vector);
}

glm::mat4 FGE::Mesh::GetMatrix ()
{
    return (m_translation_matrix * m_rotation_matrix * m_scaling_matrix);
}