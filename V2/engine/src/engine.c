#include "engine.h"

#include "cglm/cglm.h"

FGEEngine* FGEEngineNew ()
{
    FGEEngine* engine = malloc (sizeof (FGEEngine));

    return (engine);
}

FGEError FGEEngineDelete (FGEEngine* in_engine)
{
    FGEContextDelete (in_engine->context);
    FGEEventHandlerDelete (in_engine->event_handler);

    free (in_engine);

    return (0);
}

FGEError FGEEngineSetConfig (FGEEngine* in_engine, FGEEngineConfig in_config)
{
    // TODO : Clean the engine before config

    in_engine->context = FGEContextNew (in_config.title, in_config.size_w, in_config.size_h);
    in_engine->event_handler = FGEEventHandlerNew (in_engine);
    in_engine->view_type = in_config.type;
    
    glm_mat4_zero(in_engine->projection_matrix);

    in_engine->zoom_level = 1.0f;
    in_engine->cameras = FGECameraVectorNew (NULL);

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

void FGEEngineRender (FGEEngine* in_engine)
{
    return;
}