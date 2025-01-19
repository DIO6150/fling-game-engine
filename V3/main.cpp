#include <iostream>

#include "engine.hpp"
#include "game_object.hpp"

FGE::Engine* FGE::Engine::m_instance = nullptr;
std::mutex FGE::Engine::m_mtx;

int main (int argc, char** argv)
{
    FGE::Engine* engine = FGE::Engine::GetInstance();

    FGE::GameObject<std::string> m_name ("k_name_magic_wand", "Magic Wand");

    engine->PrintDebug ();

    while (!engine->ShouldWindowClose())
    {
        glfwPollEvents ();

        engine->Render ();
    }

    return (1);
}