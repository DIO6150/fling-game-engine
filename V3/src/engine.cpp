#include <iostream>
#include <string>

#include "engine.hpp"

#include "game_object.hpp"

FGE::Engine::~Engine()
{
    std::cout << "engine destroyed" << std::endl;
}

void FGE::Engine::PrintDebug ()
{
    for (auto t : m_game_objects)
    {
        for (auto e : t.second)
        {
            std::string &value = ((GameObject<std::string>*)e.second)->m_object;
            std::cout << e.first << " : " << value << std::endl;
        }
    }
}

void FGE::Engine::Render ()
{
    // Get Camera View

    // For each batches
        // bind buffers from the batches
        // use corresponding shader
        // upload uniforms
        // activate the texture atlas
        // bind texture atlas
        // glMultiDrawElementIndirect
        // cancel the shader
        // empty the batches if necessary

}

void FGE::Engine::LoadBatches ()
{
    if (m_game_objects.find ("Mesh") == m_game_objects.end ()) return;

    auto &meshes = m_game_objects["Mesh"];

    // TODO : when the renderer is more advanced, load many batches

    Batch batch;

    for (auto& it : meshes)
    {
        batch.PushMesh ((GameObject<Mesh>*)(it.second)->object)
    }

    m_batches.push_back ();
}
