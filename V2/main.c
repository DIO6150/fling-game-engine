#include <stdio.h>

#include "engine/includes/engine.h"

int main (int argc, char** argv)
{

    printf ("Hello World !\n");

    FGEEngine engine;

    FGEEngineConfig config;
    config.size_w = 400;
    config.size_h = 400;
    config.title = "assignement ?";
    config.type = FGE_PERSPECTIVE;

    FGEEngineInit (&engine, config);

    FGEVertex vertices[3] = {
        {0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0},
        {0.5, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0},
        {0.5, 0.5, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0}
    };

    FGEMesh* triangle = FGEMeshNew (vertices, (unsigned int[3]){0, 1, 2}, 3, 3, (TextureAnimation) {0});

    FGEEngineAddMesh (&engine, triangle);

    while (!FGEContextShouldClose (engine.context))
    {
        glfwPollEvents ();

        FGEEngineRender (&engine);
    }

    FGEEngineDestroy (&engine);

    return (1);
}