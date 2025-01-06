#ifndef H_MGE_Shader
#define H_MGE_Shader

#include "common.h"

typedef struct MGEShader MGEShader;

struct MGEShader
{
    unsigned int program;
    unsigned int vertex_shader;
    unsigned int fragment_shader;

    char* vertex_path;
    char* fragment_path;

    char* vertex_source;
    char* fragment_source;
};

// TODO : uniformize functions name and output

MGEShader* mgeShaderLoad (char* _in_vertex_path, char* _in_fragment_path);

MGEError mgeShaderCreate (char* _in_vertex_source, char* _in_fragment_source, MGEShader** _out_MGEShader);
MGEError mgeShaderDelete (MGEShader* _in_MGEShader);

MGEError mgeShaderCompile (MGEShader* _in_MGEShader);

MGEError mgeShaderUse (MGEShader* _in_MGEShader);
MGEError mgeShaderCancel ();

#endif