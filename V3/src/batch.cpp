
#include "engine.hpp"

#include "batch.hpp"

struct DrawCommand
{
    unsigned int count;
    unsigned int instance_count;
    unsigned int first_index;
    int base_vertex;
    unsigned int base_instance;
};

void FGE::Batch::Resize (int nb_mesh, int nb_vertex, int nb_index)
{
    void* old_vbo_data = NULL;
    void* old_ebo_data = NULL;
    void* old_dibo_data = NULL;
    void* old_matrix_ssbo_data = NULL;
    void* old_texcoords_ssbo_data = NULL;

    if (m_meshes.size() != 0)
    {

        old_vbo_data = glMapNamedBuffer(m_vbo, GL_READ_WRITE);
        old_ebo_data = glMapNamedBuffer(m_ebo, GL_READ_WRITE);
        old_dibo_data = glMapNamedBuffer(m_dibo, GL_READ_WRITE);
        old_matrix_ssbo_data = glMapNamedBuffer(m_matrix_ssbo, GL_READ_WRITE);
        old_texcoords_ssbo_data = glMapNamedBuffer(m_tex_coord_ssbo, GL_READ_WRITE);

        if (!old_vbo_data || !old_ebo_data || !old_dibo_data || !old_matrix_ssbo_data || !old_texcoords_ssbo_data)
        {
            throw (std::invalid_argument ("buffers are Null"));
        }
    }

    m_max_mesh_count += nb_mesh;
    m_max_vertex_count += nb_vertex;
    m_max_index_count += nb_index;

    glNamedBufferData (m_vbo, m_max_vertex_count * sizeof (Vertex), NULL, GL_DYNAMIC_DRAW);
    glNamedBufferSubData (m_vbo, 0, m_vertex_count * sizeof (Vertex), old_vbo_data);

    glNamedBufferData (m_ebo, m_max_index_count * sizeof (int), NULL, GL_DYNAMIC_DRAW);
    glNamedBufferSubData (m_ebo, 0, m_index_count * sizeof (int), old_ebo_data);

    glNamedBufferData (m_dibo, m_max_mesh_count * sizeof (DrawCommand), NULL, GL_DYNAMIC_DRAW);
    glNamedBufferSubData (m_dibo, 0, m_meshes.size () * sizeof (DrawCommand), old_dibo_data);

    glNamedBufferData (m_matrix_ssbo, m_max_mesh_count * sizeof (float) * 16, NULL, GL_DYNAMIC_DRAW);
    glNamedBufferSubData (m_matrix_ssbo, 0, m_meshes.size () * sizeof (float) * 16, old_matrix_ssbo_data);

    glNamedBufferData (m_tex_coord_ssbo, m_max_mesh_count * sizeof (glm::vec4), NULL, GL_DYNAMIC_DRAW);
    glNamedBufferSubData (m_tex_coord_ssbo, 0, m_meshes.size () * sizeof (glm::vec4), old_texcoords_ssbo_data);
}

FGE::Batch::Batch (std::string shader_key, std::string atlas_key)
{
    m_shader = Engine::FindGameObject<Shader> (shader_key);
    m_atlas = Engine::FindGameObject<TextureAtlas> (atlas_key);

    if (! (m_shader && m_atlas)) return;

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

FGE::Batch::~Batch ()
{

}

void FGE::Batch::BindBuffers ()
{
    glBindVertexArray (m_vao);
    glBindBuffer (GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBindBuffer (GL_DRAW_INDIRECT_BUFFER, m_dibo);
    glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 0, m_matrix_ssbo);
    glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 1, m_tex_coord_ssbo);
}

void FGE::Batch::UnbindBuffers ()
{
    glBindBuffer (GL_ARRAY_BUFFER, 0);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer (GL_DRAW_INDIRECT_BUFFER, 0);
    glBindBuffer (GL_SHADER_STORAGE_BUFFER, 0);
    glBindTexture (GL_TEXTURE_2D, 0);
}

void FGE::Batch::PushMesh (Mesh* mesh)
{
    if (m_vertex_count + mesh->GetVertices ().size () >= m_max_vertex_count || m_index_count + mesh->GetIndices ().size () >= m_max_index_count)
        Resize (1, mesh->GetVertices ().size (), mesh->GetIndices ().size ());

    mesh->SetBatch (this);
    mesh->SetMatrixOffset (m_meshes.size () * sizeof (float) * 16);
    mesh->SetPositionOffset (m_meshes.size ());

    // TODO : Take into account instance rendering
    DrawCommand command;
    command.count = mesh->GetIndices ().size ();
    command.instance_count = 1;
    command.first_index = m_index_count;
    command.base_vertex = m_vertex_count;
    command.base_instance = 0;

    glNamedBufferSubData (m_vbo, m_vertex_count * sizeof (Vertex), mesh->GetVertices ().size () * sizeof (Vertex), mesh->GetVertices ().data ());
    glNamedBufferSubData (m_ebo, m_index_count * sizeof (int), mesh->GetIndices ().size () * sizeof (int), mesh->GetIndices ().data ());
    glNamedBufferSubData (m_dibo, m_meshes.size () * sizeof (DrawCommand), sizeof (DrawCommand), &command);

    glm::mat4 model_matrix = mesh->GetMatrix ();
    glBindBuffer (GL_SHADER_STORAGE_BUFFER, m_matrix_ssbo);
    glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 0, m_matrix_ssbo);
    glNamedBufferSubData (m_matrix_ssbo, m_meshes.size () * sizeof (float) * 16, sizeof (float) * 16, glm::value_ptr(model_matrix));

    TextureQuad* quad = mesh->GetQuad ();
    float width =  (float) (quad->w ) / quad->atlas->GetMaxSize ();
    float height = (float) (quad->h ) / quad->atlas->GetMaxSize ();
    float x = (float) (quad->x ) / quad->atlas->GetMaxSize ();
    float y = (float) (quad->y ) / quad->atlas->GetMaxSize ();
    glm::vec4 coords_ratio = { x, y, width, height };

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_tex_coord_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_tex_coord_ssbo);
    glNamedBufferSubData(m_tex_coord_ssbo, m_meshes.size () * sizeof(glm::vec4), sizeof(glm::vec4), &coords_ratio[0]);

    m_meshes.push_back (mesh);
    m_index_count += mesh->GetIndices ().size ();
    m_vertex_count += mesh->GetVertices ().size ();
}

void FGE::Batch::UpdateMatrix (unsigned int matrix_offset, glm::mat4 matrix)
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_matrix_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_matrix_ssbo);
    glNamedBufferSubData(m_matrix_ssbo, matrix_offset, sizeof (float) * 16, &matrix[0][0]);
}

size_t FGE::Batch::GetMeshCount ()
{
    return (m_meshes.size ());
}

FGE::Shader* FGE::Batch::GetShader ()
{
    return (m_shader);
}

unsigned int FGE::Batch::GetAtlasObject ()
{
    return (m_atlas->GetObject());
}