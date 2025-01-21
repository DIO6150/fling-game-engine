#include <iostream>
#include <string>

#include "engine.hpp"

FGE::Engine* FGE::Engine::m_instance = nullptr;
std::mutex FGE::Engine::m_mtx;

FGE::Engine::~Engine()
{
    std::cout << "engine destroyed" << std::endl;

    for (auto it : m_game_objects)
    {
        delete it.second;
    }
}

bool FGE::Engine::ShouldWindowClose ()
{
    return (glfwWindowShouldClose ( Engine::GetInstance()->m_context->GetWindow()));
}

std::shared_ptr<FGE::Scene> FGE::Engine::GetActiveScene () 
{
    return (Engine::GetInstance()->active_scene);
}

void FGE::Engine::CreateBatch (std::string shader_key, std::string atlas_key)
{
    Engine* engine = Engine::GetInstance();

    engine->m_batches.push_back (Batch (shader_key, atlas_key));
}

FGE::Mesh* FGE::Engine::UploadMesh (std::string key, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scaling)
{
    return (FGE::Engine::GetActiveScene ()->UploadMesh (key, translation, rotation, scaling));
}

void FGE::Engine::UpdateScene (std::vector<FGE::Mesh*> updated)
{
    for (auto& it : updated)
    {
        Batch* batch = it->GetBatch ();
        unsigned int matrix_offset = it->GetMatrixOffset ();
        glm::mat4 matrix = it->GetMatrix ();

        batch->UpdateMatrix (matrix_offset, matrix);
    }
}

void FGE::Engine::LoadBatches (std::string shader_key)
{
    Engine* engine = Engine::GetInstance();

    // TODO auto batch id

    Batch* batch = &engine->m_batches.back ();

    for (Mesh& it : engine->active_scene->GetMeshes() )
    {
        // just to see if it works
        if (!it.GetQuad()) { it.SetTexture ("k_atlas", ""); }
        batch->PushMesh (&it);
    }

}


void FGE::Engine::Render ()
{
    Engine* engine = Engine::GetInstance();

    // TODO : Camera class
    glm::mat4 view;
    view = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians (45.0f), 1.0f, 0.1f, 100.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // For each batches
    for (Batch& batch : engine->m_batches)
    {
        // bind buffers from the batches
        batch.BindBuffers ();

        // use corresponding shader
        batch.GetShader ()->Use ();

        // upload uniforms
        batch.GetShader ()-> UploadMatrix4 ("uProjection", projection);
        batch.GetShader ()-> UploadMatrix4 ("uView", view);

        // TODO: manage multiple atlases

        batch.GetShader ()-> Upload1i ("uAtlas", 0);

        // activate the texture atlas
        glActiveTexture (GL_TEXTURE0);
        // bind texture atlas
        glBindTexture (GL_TEXTURE_2D_ARRAY, batch.GetAtlasObject ());

        
        // glMultiDrawElementIndirect
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, NULL, batch.GetMeshCount (), 0);
        
        // cancel the shader
        batch.GetShader ()->Cancel ();

        // empty the batches if necessary

        batch.UnbindBuffers ();
    }

    glfwSwapBuffers(engine->m_context->GetWindow());

}

void FGE::Engine::TranslateMesh (Mesh* mesh, glm::vec3 vector, bool keep)
{
    if (keep) mesh->Translate (vector);
    else mesh->SetTranslate (vector);
    FGE::Engine::UpdateScene ({mesh});
}

void FGE::Engine::RotateMesh (Mesh* mesh, glm::vec3 vector, bool keep)
{
    if (keep) mesh->Rotate (vector);
    else mesh->SetRotate (vector);
    FGE::Engine::UpdateScene ({mesh});
}

void FGE::Engine::ScaleMesh (Mesh* mesh, glm::vec3 vector, bool keep)
{
    if (keep) mesh-> Scale (vector);
    else mesh->SetScale (vector);
    FGE::Engine::UpdateScene ({mesh});
}