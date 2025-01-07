#ifndef HEADER_FGE_ENGINE
#define HEADER_FGE_ENGINE

#include "cglm/cglm.h"

#include "common/common.h"
#include "common/vector_impl.h"

#include "camera.h"
#include "context.h"
#include "batch.h"
#include "event_handler.h"
#include "tex_array.h"

/*
    The main class of the lib
*/

typedef struct FGEEngine FGEEngine;
typedef struct FGEEngineConfig FGEEngineConfig;

struct FGEEngine
{
    FGEContext* context;
    FGEEventHandler* event_handler;

    FGEViewType view_type;
    mat4 projection_matrix;
    float zoom_level;

    FGECameraVector cameras;
    FGECamera* active_camera;

    FGEBatchVector batches;

    FGETexArray atlas;
    FGEMeshVector meshes;
    // FGEVector animations;
};

struct FGEEngineConfig
{
    char* title;
    int size_w;
    int size_h;
    FGEViewType type;
};

FGEEngine* FGEEngineNew ();
FGEError FGEEngineDelete (FGEEngine* in_engine);

FGEError FGEEngineSetConfig (FGEEngine* in_engine, FGEEngineConfig in_config);

FGEError FGEEngineConfigureBatch (FGEEngine* in_engine, int in_index, FGEBatchParam in_param);

FGEError FGEEngineLoadTexture (FGEEngine* in_engine, const char* in_path);

void FGEEngineResizeWindow (FGEEngine* in_engine);

void FGEEngineZoom (FGEEngine* in_engine, float in_zoom);
void FGEEngineSetZoom (FGEEngine* in_engine, float in_zoom);

void FGEEngineRender (FGEEngine* in_engine);


#endif