#include "batch.hpp"

#include "engine.hpp"

FGE::Batch::Batch (std::string shader_key)
{
    m_shader = FGE::Engine::FindGameObject<Shader> (shader_key);

    glGenVertexArrays(1, &m_vao);

    glGenBuffers (1, &m_vbo);
    glGenBuffers (1, &m_ebo);
    glGenBuffers (1, &m_dibo);

    glGenBuffers (1, &m_matrix_ssbo);
    glGenBuffers (1, &m_tex_coord_ssbo);

    BindBuffers ();

    glNamedBufferData (m_vbo, 1, NULL, GL_DYNAMIC_DRAW);
    glNamedBufferData (m_ebo, 1, NULL, GL_DYNAMIC_DRAW);
    glNamedBufferData (m_dibo, 1, NULL, GL_DYNAMIC_DRAW);

    glBindBuffer (GL_SHADER_STORAGE_BUFFER, m_matrix_ssbo);
    glNamedBufferData (m_matrix_ssbo, 1, NULL, GL_DYNAMIC_DRAW);

    glBindBuffer (GL_SHADER_STORAGE_BUFFER, m_tex_coord_ssbo);
    glNamedBufferData (m_tex_coord_ssbo, 1, NULL, GL_DYNAMIC_DRAW);

    // TODO : Allow custom vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (float), (void*)(0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (float), (void*)(3 * sizeof (float)));

    glEnableVertexAttribArray (0);
    glEnableVertexAttribArray (1);

    UnbindBuffers ();
}