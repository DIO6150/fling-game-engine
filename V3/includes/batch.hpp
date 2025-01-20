#pragma once

#include "glad/glad.h"

#include "mesh.hpp"
#include "shader.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <stdexcept>
#include <string>

namespace FGE
{
    struct DrawCommand
    {
        unsigned int count;
        unsigned int instance_count;
        unsigned int first_index;
        int base_vertex;
        unsigned int base_instance;
    };

    class Batch
    {
        unsigned int m_vao;
        unsigned int m_vbo;
        unsigned int m_ebo;
        unsigned int m_dibo;
        unsigned int m_matrix_ssbo;
        unsigned int m_tex_coord_ssbo;

        Shader* m_shader = nullptr;

        unsigned int m_index_count = 0;
        unsigned int m_vertex_count = 0;
        std::vector<Mesh*> m_meshes;

        unsigned int m_max_index_count = 0;
        unsigned int m_max_vertex_count = 0;
        unsigned int m_max_mesh_count = 0;
    
    public:
        Batch(std::string shader_key);

        ~Batch()
        {

        }

        void BindBuffers ()
        {
            glBindVertexArray (m_vao);
            glBindBuffer (GL_ARRAY_BUFFER, m_vbo);
            glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, m_ebo);
            glBindBuffer (GL_DRAW_INDIRECT_BUFFER, m_dibo);
            glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 0, m_matrix_ssbo);
            glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 1, m_tex_coord_ssbo);
        }

        void UnbindBuffers ()
        {
            glBindBuffer (GL_ARRAY_BUFFER, 0);
            glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindBuffer (GL_DRAW_INDIRECT_BUFFER, 0);
            glBindBuffer (GL_SHADER_STORAGE_BUFFER, 0);
            glBindTexture (GL_TEXTURE_2D, 0);
        }

        void Resize (int new_size, int new_vertex_size, int new_index_size)
        {
            void* old_vbo_data = NULL;
            void* old_ebo_data = NULL;
            void* old_dibo_data = NULL;
            void* old_matrix_ssbo_data = NULL;
            void* old_texcoords_ssbo_data = NULL;

            // TODO : There is actually undefined behavior here,
            // If I get old data with old size into an old size container then fuck me I guess ?
            // Because the data packed next to it is not mine and therefore i'm shit

            // 19/01/25 :
            // Is it still valid as of now ?
            // I don't remember it has been like 6 months now urghh

            if (m_meshes.size() != 0)
            {

                old_vbo_data = glMapNamedBuffer(m_vbo, GL_READ_WRITE);
                old_ebo_data = glMapNamedBuffer(m_ebo, GL_READ_WRITE);
                old_dibo_data = glMapNamedBuffer(m_dibo, GL_READ_WRITE);
                old_matrix_ssbo_data = glMapNamedBuffer(m_matrix_ssbo, GL_READ_WRITE);
                old_texcoords_ssbo_data = glMapNamedBuffer(m_tex_coord_ssbo, GL_READ_WRITE);

                if (!old_vbo_data || !old_ebo_data || !old_dibo_data || !old_matrix_ssbo_data || !old_texcoords_ssbo_data)
                {
                    /*
                    if (old_vbo_data)
                        glUnmapNamedBuffer(m_vbo);

                    if (old_ebo_data)
                        glUnmapNamedBuffer(m_ebo);

                    if (old_dibo_data)
                        glUnmapNamedBuffer(m_dibo);

                    if (old_matrix_ssbo_data)
                        glUnmapNamedBuffer(m_matrix_ssbo);

                    if (old_texcoords_ssbo_data)
                        glUnmapNamedBuffer(m_tex_coord_ssbo);
                    */

                    throw std::invalid_argument ("buffers are Null");
                }
            }

            m_max_mesh_count += new_size;
            m_max_vertex_count += new_vertex_size;
            m_max_index_count += new_index_size;

            glNamedBufferData (m_vbo, m_max_vertex_count * sizeof (Vertex), NULL, GL_DYNAMIC_DRAW);
            glNamedBufferSubData (m_vbo, 0, m_vertex_count * sizeof (Vertex), old_vbo_data);

            glNamedBufferData (m_ebo, m_max_index_count * sizeof (int), NULL, GL_DYNAMIC_DRAW);
            glNamedBufferSubData (m_ebo, 0, m_index_count * sizeof (int), old_ebo_data);

            glNamedBufferData (m_dibo, m_max_mesh_count * sizeof (DrawCommand), NULL, GL_DYNAMIC_DRAW);
            glNamedBufferSubData (m_dibo, 0, m_meshes.size () * sizeof (DrawCommand), old_dibo_data);

            glNamedBufferData (m_matrix_ssbo, m_max_mesh_count * sizeof (glm::mat4), NULL, GL_DYNAMIC_DRAW);
            glNamedBufferSubData (m_matrix_ssbo, 0, m_meshes.size () * sizeof (glm::mat4), old_matrix_ssbo_data);

            glNamedBufferData (m_tex_coord_ssbo, m_max_mesh_count * sizeof (glm::vec4), NULL, GL_DYNAMIC_DRAW);
            glNamedBufferSubData (m_tex_coord_ssbo, 0, m_meshes.size () * sizeof (glm::vec4), old_texcoords_ssbo_data);
        }

        void PushMesh (Mesh* mesh)
        {

            if (m_vertex_count + mesh->GetVertices ().size () >= m_max_vertex_count || 
                m_index_count + mesh->GetIndices ().size () >= m_max_index_count)
            {
                Resize (1, mesh->GetVertices ().size (), mesh->GetIndices ().size ());

            }


            // TODO : Take into account instance rendering
            DrawCommand command;

            command.count = mesh->GetIndices ().size ();
            command.instance_count = 1;
            command.first_index = m_index_count;
            command.base_vertex = m_vertex_count;
            command.base_instance = 0;

            // Vertex Data
            glNamedBufferSubData (m_vbo, m_vertex_count * sizeof (Vertex), mesh->GetVertices ().size () * sizeof (Vertex), mesh->GetVertices ().data ());

            // Index Data
            glNamedBufferSubData (m_ebo, m_index_count * sizeof (int), mesh->GetIndices ().size () * sizeof (int), mesh->GetIndices ().data ());

            // Draw Command Data
            glNamedBufferSubData (m_dibo, m_meshes.size () * sizeof (DrawCommand), sizeof (DrawCommand), &command);

            // Matrix Data
            glm::mat4 model_matrix = mesh->GetMatrix ();
            glBindBuffer (GL_SHADER_STORAGE_BUFFER, m_matrix_ssbo);
            glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 0, m_matrix_ssbo);
            glNamedBufferSubData (m_matrix_ssbo, m_meshes.size () * sizeof (glm::mat4), sizeof (glm::mat4), glm::value_ptr(model_matrix));

            // TODO : see V1 for texture atlas logic (to implement)
            // {
            //
            //
            // }

            m_meshes.push_back (mesh);
            m_index_count += mesh->GetIndices ().size ();
            m_vertex_count += mesh->GetVertices ().size ();
        }

        size_t GetVertexCount () { return (m_meshes.size ()); }
        Shader* GetShader () { return (m_shader); }
    };
}