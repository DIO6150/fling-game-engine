#ifndef H_MGE_TEXTURE
#define H_MGE_TEXTURE


typedef struct
{
    unsigned int object;

    int width;
    int height;
    int channel_count;
} TextureObject;

int mgeTextureObjectInit(TextureObject** _out_texture_object);
int mgeTextureObjectFree(TextureObject* _in_texture_object);


typedef struct
{
    TextureObject* object;
    unsigned char* data;
} Texture2D;

int mgeTexture2DCreate(Texture2D** _out_texture);
int mgeTexture2DFree(Texture2D* _in_texture);

int mgeTexture2DLoad(const char* _in_path, Texture2D** _out_texture);
int mgeTexture2DGenerateFlat(Texture2D** _out_texture, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a);


typedef struct
{
    char* path;
    double x, y, w, h;

} TextureQuad;


#define MGE_TEXTURE_ARRAY_2D_SIZE 512

typedef struct
{
    unsigned int object;

    int width, height;
    int cumul_width, cumul_height;
    int max_height;

    TextureQuad** quads;
    int quads_count;
    int quads_max_count;
} TextureArray2D;

int mgeTextureArray2DInit(TextureArray2D** _out_texture_array);
int mgeTextureArray2DFree(TextureArray2D* _in_texture_array);

int mgeTextureArray2DCreateQuad(TextureQuad** _out_quad, char* _in_path, TextureArray2D* _in_texture_array);


#endif