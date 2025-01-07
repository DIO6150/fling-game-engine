#ifndef HEADER_FGE_TEX_ARRAY
#define HEADER_FGE_TEX_ARRAY

#include "common/common.h"

#include "common/string.h"

typedef struct FGETexQuad FGETexQuad;
typedef struct FGETexArray FGETexArray;

struct FGETexQuad
{
    FGEString* path;
    double x, y, w, h;
};

struct FGETexArray
{
    unsigned int object;

    int width, height;
    int cumul_width, cumul_height;
    int max_height;

    FGETexQuad** quads;
    int quads_count;
    int quads_max_count;
};

FGEError FGETexArrayInit (FGETexArray* _out_texture_array);
FGEError FGETexArrayFree (FGETexArray* in_texture_array);

FGETexQuad* FGETexArrayGetQuad (FGETexArray* in_texture_array, char* in_path);

#define FGE_TEXTURE_ARRAY_2D_SIZE 512

#endif