#include "engine.h"

#include "cglm/cglm.h"

FGEError FGEEngineInit (FGEEngine* out_engine, FGEEngineConfig in_config)
{
    out_engine->context = FGEContextNew (in_config.title, in_config.size_w, in_config.size_h);
    out_engine->event_handler = FGEEventHandlerNew (out_engine);
    out_engine->view_type = in_config.type;
    
    glm_mat4_zero (out_engine->projection_matrix);

    out_engine->zoom_level = 1.0f;
    out_engine->cameras = FGECameraVectorNew (NULL);

    return (FGE_E_SUCCESS);
}

FGEError FGEEngineDestroy (FGEEngine* in_engine)
{
    FGEContextDelete (in_engine->context);
    FGEEventHandlerDelete (in_engine->event_handler);

    return (0);
}

FGEError FGEEngineSetConfig (FGEEngine* in_engine, FGEEngineConfig in_config)
{
    // TODO : Clean the engine before config

    return (FGE_E_SUCCESS);
}

FGEError FGEEngineConfigureBatch (FGEEngine* in_engine, int in_index, FGEBatchParam in_param)
{
    return (0);
}

FGEError FGEEngineLoadTexture (FGEEngine* in_engine, const char* in_path)
{
    return (0);
}

void FGEEngineResizeWindow (FGEEngine* in_engine)
{
    return;
}

void FGEEngineZoom (FGEEngine* in_engine, float in_zoom)
{
    return;
}

void FGEEngineSetZoom (FGEEngine* in_engine, float in_zoom)
{
    return;
}

void FGEEngineAddMesh (FGEEngine* in_engine, FGEMesh* mesh)
{
    FGEMeshVectorPush (&in_engine->meshes, mesh);
}

void FGEEngineRender (FGEEngine* in_engine)
{
    return;
}