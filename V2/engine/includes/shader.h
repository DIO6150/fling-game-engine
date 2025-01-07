#ifndef HEADER_FGE_Shader
#define HEADER_FGE_Shader

#include "common/common.h"

typedef struct FGEShader FGEShader;

struct FGEShader
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


FGEShader* FGEShaderNew (char* in_vertex_source, char* in_fragment_source);
FGEError FGEShaderDelete (FGEShader* in_FGEShader);

FGEShader* FGEShaderLoad (char* in_vertex_path, char* in_fragment_path);

FGEError FGEShaderCompile (FGEShader* in_FGEShader);

FGEError FGEShaderUse (FGEShader* in_FGEShader);
FGEError FGEShaderCancel ();

#endif