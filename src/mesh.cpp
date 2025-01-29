#include "mesh.hpp"

#include "engine.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>


FGE::Mesh::Mesh (const Mesh &other)
{
    m_vertices = other.m_vertices;
    m_indices = other.m_indices;
    
    m_translation_matrix = other.m_translation_matrix;
    m_rotation_matrix = other.m_rotation_matrix;
    m_origin_matrix = other.m_origin_matrix;
    m_scaling_matrix = other.m_scaling_matrix;

    m_mesh_meta = other.m_mesh_meta;
    m_quad = other.m_quad;
}

FGE::Mesh::Mesh (Mesh&& other)
{
    m_vertices = std::move(other.m_vertices);
    m_indices = std::move(other.m_indices);

    m_translation_matrix = other.m_translation_matrix;
    m_rotation_matrix = other.m_rotation_matrix;
    m_origin_matrix = other.m_origin_matrix;
    m_scaling_matrix = other.m_scaling_matrix;

    m_mesh_meta = other.m_mesh_meta;
    m_quad = other.m_quad;

    other.m_quad = nullptr;
}

FGE::Mesh& FGE::Mesh::operator=(const Mesh& other)
{
    m_vertices = other.m_vertices;
    m_indices = other.m_indices;

    m_translation_matrix = other.m_translation_matrix;
    m_rotation_matrix = other.m_rotation_matrix;
    m_origin_matrix = other.m_origin_matrix;
    m_scaling_matrix = other.m_scaling_matrix;

    m_mesh_meta = other.m_mesh_meta;
    m_quad = other.m_quad;

    return (*this);
}

FGE::Mesh::Mesh (std::vector<Vertex> vertices, std::vector<unsigned int> indices, glm::mat4 origin)
{
    m_vertices = std::make_shared<std::vector<Vertex>> (vertices);
    m_indices = std::make_shared<std::vector<unsigned int>> (indices);

    m_translation_matrix = glm::mat4 (1.0f);
    m_rotation_matrix = glm::mat4 (1.0f);
    m_origin_matrix = origin;
    m_scaling_matrix = glm::mat4 (1.0f);
}

FGE::Mesh::~Mesh ()
{
    
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

void FGE::Mesh::Origin(glm::vec3 vector)
{
    m_origin_matrix = glm::translate (m_origin_matrix, vector);
}

void FGE::Mesh::SetTranslate(glm::vec3 vector)
{
    m_translation_matrix = glm::translate (glm::mat4 (1.0f), vector);
}

void FGE::Mesh::SetRotate(glm::vec3 vector)
{
    m_rotation_matrix = glm::rotate (glm::mat4 (1.0f), glm::radians (vector.x), {1.0, 0.0, 0.0});
    m_rotation_matrix = glm::rotate (m_rotation_matrix, glm::radians (vector.y), {0.0, 1.0, 0.0});
    m_rotation_matrix = glm::rotate (m_rotation_matrix, glm::radians (vector.z), {0.0, 0.0, 1.0});
}

void FGE::Mesh::SetOrigin(glm::vec3 vector)
{
    m_origin_matrix = glm::translate (glm::mat4 (1.0f), vector);
}

void FGE::Mesh::SetScale(glm::vec3 vector)
{
    m_scaling_matrix = glm::scale (glm::mat4 (1.0f), vector);
}

void FGE::Mesh::SetTexture (std::string atlas_key, std::string texture_key)
{
    m_quad = Engine::FindGameObject<TextureAtlas> (atlas_key)->GetQuad (texture_key);
}

FGE::TextureQuad* FGE::Mesh::GetQuad ()
{
    return (m_quad);
}

glm::mat4 FGE::Mesh::GetMatrix ()
{
    return ( m_translation_matrix * m_rotation_matrix * m_origin_matrix * m_scaling_matrix);
}

FGE::Batch* FGE::Mesh::GetBatch ()
{
    return (m_mesh_meta.m_batch);
}

unsigned int FGE::Mesh::GetPositionOffset ()
{
    return (m_mesh_meta.m_position);
}

unsigned int FGE::Mesh::GetMatrixOffset ()
{
    return (m_mesh_meta.m_matrix_offset);
}

void FGE::Mesh::SetBatch (Batch* batch)
{
    m_mesh_meta.m_batch = batch;
}

void FGE::Mesh::SetPositionOffset (unsigned int position_offset)
{
    m_mesh_meta.m_position = position_offset;
}

void FGE::Mesh::SetMatrixOffset (unsigned int matrix_offset)
{
    m_mesh_meta.m_matrix_offset = matrix_offset;
}