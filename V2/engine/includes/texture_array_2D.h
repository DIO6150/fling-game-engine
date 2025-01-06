#ifndef H_MGE_TEXTURE
#define H_MGE_TEXTURE

#include "common.h"

#define MGE_TEXTURE_ARRAY_2D_SIZE 512

typedef struct MGETextureQuad MGETextureQuad;
typedef struct MGETextureArray2D MGETextureArray2D;

struct MGETextureQuad
{
    char** path;
    double x, y, w, h;
};

struct MGETextureArray2D
{
    unsigned int object;

    int width, height;
    int cumul_width, cumul_height;
    int max_height;

    MGETextureQuad** quads;
    int quads_count;
    int quads_max_count;
};

MGEError mgeTextureArray2DInit (MGETextureArray2D* _out_texture_array);
MGEError mgeTextureArray2DFree (MGETextureArray2D* _in_texture_array);

MGETextureQuad* mgeTextureArray2DGetQuad (MGETextureArray2D* _in_texture_array, char* _in_path);

#endif