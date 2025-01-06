#ifndef H_MGE_ENGINE
#define H_MGE_ENGINE

#include "cglm/cglm.h"

#include "camera.h"
#include "common.h"
#include "context.h"
#include "batch.h"
#include "event_manager.h"
#include "texture_array_2D.h"

/*
    The main class of the lib
*/

typedef struct MGEEngine MGEEngine;
typedef struct MGEEngineConfig MGEEngineConfig;

struct MGEEngine
{
    MGEContext context;
    MGEEventManager event_manager;

    MGEViewType view_type;
    mat4 projection_matrix;
    float zoom;

    MGEArray cameras;
    MGECamera* active_camera;

    MGEArray batches;

    MGETextureArray2D atlas;
    MGEArray meshes;
    MGEArray animations;
};

struct MGEEngineConfig
{
    char* title;
    int size_w;
    int size_h;
    MGEViewType type;
};

MGEEngine mgeEngineInit ();
MGEError mgeEngineFree (MGEEngine* _in_engine);

MGEError mgeEngineSetConfig (MGEEngine* _in_engine, MGEEngineConfig* _in_config);

MGEError mgeEngineConfigureBatch (MGEEngine* _in_engine, int _in_index, MGEBatchParam _in_param);

MGEError mgeEngineLoadTexture (MGEEngine* _in_engine, const char* _in_path);

void mgeEngineResizeWindow (MGEEngine* _in_engine);

void mgeEngineZoom (MGEEngine* _in_engine, float _in_zoom);
void mgeEngineSetZoom (MGEEngine* _in_engine, float _in_zoom);

void mgeEngineRender (MGEEngine* _in_engine);


#endif