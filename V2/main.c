#include <stdio.h>

#include "engine/includes/engine.h"

int main (int argc, char** argv)
{

    printf ("Hello World !\n");

    FGEEngine* engine = FGEEngineNew ();

    FGEEngineConfig config;
    config.size_w = 400;
    config.size_h = 400;
    config.title = "assignement ?";
    config.type = FGE_PERSPECTIVE;

    FGEEngineSetConfig (engine, config);

    while (!FGEContextShouldClose (engine->context))
    {
        glfwPollEvents ();
    }

    FGEEngineDelete (engine);

    return (1);
}