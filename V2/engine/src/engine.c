#include "engine.h"



MGEEngine mgeEngineInit ()
{
    MGEEngine* engine = malloc (sizeof (MGEEngine));
}

MGEError mgeEngineFree (MGEEngine* _in_engine)
{

}

MGEError mgeEngineSetConfig (MGEEngine* _in_engine, MGEEngineConfig _in_config)
{
    // TODO : Clean the engine before config

    _in_engine->context = mgeContextInit(_in_config.title, _in_config.size_w, _in_config.size_h);
    _in_engine->event_manager = mgeEventManagerInit(_in_engine);
    _in_engine->view_type = _in_config.type;
    _in_engine->projection_matrix; // brrrr
    _in_engine->zoom = 1.0f;
    _in_engine->cameras = mgeArrayInit(sizeof (MGECamera));
}

MGEError mgeEngineConfigureBatch (MGEEngine* _in_engine, int _in_index, MGEBatchParam _in_param)
{

}

MGEError mgeEngineLoadTexture (MGEEngine* _in_engine, const char* _in_path)
{

}

void mgeEngineResizeWindow (MGEEngine* _in_engine)
{

}

void mgeEngineZoom (MGEEngine* _in_engine, float _in_zoom);
{

}

void mgeEngineSetZoom (MGEEngine* _in_engine, float _in_zoom)*
{

}

void mgeEngineRender (MGEEngine* _in_engine)
{

}