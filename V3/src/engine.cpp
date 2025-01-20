#include <iostream>
#include <string>

#include "engine.hpp"


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

void FGE::Engine::UploadMesh (std::string key, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scaling)
{
    FGE::Engine::GetActiveScene ()->UploadMesh (key, translation, rotation, scaling);
}

void FGE::Engine::LoadBatches (std::string shader_key)
{
    Engine* engine = Engine::GetInstance();

    // TODO auto batch id

    Batch batch (shader_key);

    for (Mesh& it : engine->active_scene->GetMeshes() )
    {
        batch.PushMesh (&it);
    }

    engine->m_batches.push_back (batch);
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

        // activate the texture atlas
        // bind texture atlas
        
        // glMultiDrawElementIndirect
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, NULL, batch.GetVertexCount (), 0);
        
        // cancel the shader
        batch.GetShader ()->Cancel ();

        // empty the batches if necessary

        batch.UnbindBuffers ();
    }

    glfwSwapBuffers(engine->m_context->GetWindow());

}