
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stb_image/stb_image.h"

#include "core/texture.h"

#include "glad/glad.h"


int mgeTextureObjectInit(TextureObject** _out_texture_object)
{
    *_out_texture_object = malloc(sizeof(TextureObject));

    if (!(*_out_texture_object))
    {
        printf("%s(%s) : Texture Object couldn't be initialized.\n", __func__, __FILE__);
        return (0);
    }

    (*_out_texture_object)->object = 0;

    (*_out_texture_object)->width = 0;
    (*_out_texture_object)->height = 0;

    (*_out_texture_object)->channel_count = 0;

    glGenTextures(1, &(*_out_texture_object)->object);

    return (1);
}

int mgeTextureObjectFree(TextureObject* _in_texture_object)
{
    if (!_in_texture_object)
    {
        printf("%s(%s) : Texture Object is NULL.\n", __func__, __FILE__);
        return (0);
    }

    free(_in_texture_object);

    return (1);
}

int mgeTexture2DLoad(const char* _in_path, Texture2D** _out_texture)
{
    if (!mgeTexture2DCreate(_out_texture))
    {
        printf("%s(%s) : Failed to initialize texture object, aborting.\n", __func__, __FILE__);
        return (0);
    }

	glBindTexture(GL_TEXTURE_2D, (*_out_texture)->object->object);

    stbi_set_flip_vertically_on_load(1);

	(*_out_texture)->data = stbi_load(_in_path,
		&(*_out_texture)->object->width,
		&(*_out_texture)->object->height,
		&(*_out_texture)->object->channel_count,
		4);

	if (!(*_out_texture)->data)
	{
		printf("%s(%s) : (stbi_image) %s\n", __func__, __FILE__, stbi_failure_reason());
		return (0);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (*_out_texture)->object->width, (*_out_texture)->object->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (*_out_texture)->data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

    return (1);
}

int mgeTexture2DCreate(Texture2D** _out_texture)
{
    *_out_texture = malloc(sizeof(Texture2D));

    if (!(*_out_texture))
    {
        printf("%s(%s) : Texture couldn't be initialized.\n", __func__, __FILE__);
        return (0);
    }

    if (!mgeTextureObjectInit(&(*_out_texture)->object))
    {
        free(*_out_texture);
        printf("%s(%s) : Failed to initialize texture object, aborting.\n", __func__, __FILE__);
        return (0);
    }

    (*_out_texture)->data = NULL;

    return (1);
}

int mgeTexture2DFree(Texture2D* _in_texture)
{
    if (!_in_texture)
    {
        printf("%s(%s) : Texture is NULL.\n", __func__, __FILE__);
        return (0);
    }

    if (!_in_texture->object)
    {
        free(_in_texture);
        printf("%s(%s) : Texture Object is NULL.\n", __func__, __FILE__);
        return (0);
    }

	stbi_image_free(_in_texture->data);

    free(_in_texture->object);
    free(_in_texture);
    return (1);
}

int mgeTexture2DGenerateFlat(Texture2D** _out_texture, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    if (!mgeTexture2DCreate(_out_texture))
    {
        printf("%s(%s) : Failed to initialize texture object, aborting.\n", __func__, __FILE__);
        return (0);
    }

    (*_out_texture)->data = malloc(sizeof (unsigned char) * width * height * 4);

    for (int i = 0; i < width * height * 4; i ++)
    {
             if (i % 4  == 0) (*_out_texture)->data[i] = r;
        else if (i % 4  == 1) (*_out_texture)->data[i] = g;
        else if (i % 4  == 2) (*_out_texture)->data[i] = b;
        else if (i % 4  == 3) (*_out_texture)->data[i] = a;
    }

    (*_out_texture)->object->width = width;
    (*_out_texture)->object->height = height;

	glBindTexture(GL_TEXTURE_2D, (*_out_texture)->object->object);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (*_out_texture)->object->width, (*_out_texture)->object->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (*_out_texture)->data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

    return (1);
}

static unsigned char* create_flat_color(int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    unsigned char* data = malloc(sizeof (unsigned char) * width * height * 4);

    for (int i = 0; i < width * height * 4; i ++)
    {
             if (i % 4  == 0) data[i] = r;
        else if (i % 4  == 1) data[i] = g;
        else if (i % 4  == 2) data[i] = b;
        else if (i % 4  == 3) data[i] = a;
    }

    return data;
}

int mgeTextureArray2DInit(TextureArray2D** _out_texture_array)
{
    (*_out_texture_array) = malloc(sizeof (TextureArray2D));

    if (!(*_out_texture_array))
    {
        printf("%s(%s) : Couldn't initialize texture array.\n", __func__, __FILE__);
        return (0);
    }

    (*_out_texture_array)->object = 0;

    (*_out_texture_array)->width = 0;
    (*_out_texture_array)->height = 0;

    (*_out_texture_array)->cumul_width = 0;
    (*_out_texture_array)->cumul_height = 0;

    (*_out_texture_array)->max_height = 0;

    (*_out_texture_array)->quads = NULL;
    (*_out_texture_array)->quads_count = 0;
    (*_out_texture_array)->quads_max_count = 16;

    (*_out_texture_array)->quads = malloc( sizeof (TextureQuad*) * 16);

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &(*_out_texture_array)->object);
	glBindTexture(GL_TEXTURE_2D_ARRAY, (*_out_texture_array)->object);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, MGE_TEXTURE_ARRAY_2D_SIZE, MGE_TEXTURE_ARRAY_2D_SIZE, 1);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

    // Generate a texture for path of ""

    TextureQuad* empty_quad = malloc ( sizeof (TextureQuad));

    empty_quad->path = "";
    empty_quad->x = 0;
    empty_quad->y = 0;
    empty_quad->w = 2;
    empty_quad->h = 2;

    unsigned char* data = create_flat_color(2, 2, 0, 0, 0, 255);
    // its like minecraft now :3 teehee
    data[0] = 207;
    data[1] = 8;
    data[2] = 174;
    data[3] = 255;

    data[12] = 207;
    data[13] = 8;
    data[14] = 174;
    data[15] = 255;

    glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
        0,
        0, 0, 0,
        2, 2, 1,
        GL_RGBA, GL_UNSIGNED_BYTE,
        data
    );

    free(data);

    (*_out_texture_array)->quads[0] = empty_quad;
    (*_out_texture_array)->quads_count ++;

    (*_out_texture_array)->cumul_width = 2;
    (*_out_texture_array)->cumul_height = 2;
    (*_out_texture_array)->max_height = 2;


    return (1);
}

int mgeTextureArray2DCreateQuad(TextureQuad** _out_quad, char* _in_path, TextureArray2D* _in_texture_array)
{
    if (!_in_texture_array)
    {
        printf("%s(%s) : Texture Array is NULL.\n", __func__, __FILE__);
        return (0);
    }

    for (int i = 0; i < _in_texture_array->quads_count; i ++)
    {
        if ( !strcmp(_in_texture_array->quads[i]->path, _in_path) )
        {
            (*_out_quad) = _in_texture_array->quads[i];
            return (1);
        }
    }

    int width, height, nb_channels;

    stbi_set_flip_vertically_on_load(1);

    unsigned char* data = stbi_load(_in_path, &width, &height, &nb_channels, 4);

    if (_in_texture_array->max_height < height)
    {
        _in_texture_array->max_height = height;
    }

    if (!data)
    {
        printf("%s(%s) : (%s) %s\n", __func__, __FILE__, _in_path, stbi_failure_reason());
        return (0);
    }

    if (_in_texture_array->quads_count >= _in_texture_array->quads_max_count)
    {
        _in_texture_array->quads_max_count *= 2;
        _in_texture_array->quads = realloc(_in_texture_array->quads, _in_texture_array->quads_max_count * sizeof (TextureQuad));
    }

        if (_in_texture_array->cumul_width + width > MGE_TEXTURE_ARRAY_2D_SIZE)
    {
        _in_texture_array->cumul_height += _in_texture_array->max_height;
        _in_texture_array->cumul_width = 0;
        _in_texture_array->max_height = 0;
    }

    if (_in_texture_array->cumul_height + height > MGE_TEXTURE_ARRAY_2D_SIZE)
    {
        stbi_image_free(data);

        (*_out_quad) = _in_texture_array->quads[0];

        printf("%s(%s) : Too much textures into the array", __func__, __FILE__);

        return (0);
    }

    TextureQuad* quad = malloc( sizeof(TextureQuad) );

    quad->path = malloc ( sizeof(char) * ( strlen(_in_path) + 1 ));
    strcpy(quad->path, _in_path);
    quad->x = _in_texture_array->cumul_width;
    quad->y = _in_texture_array->cumul_height;
    quad->w = width;
    quad->h = height;

    (*_out_quad) = quad;

    _in_texture_array->quads[_in_texture_array->quads_count] = quad;
    _in_texture_array->quads_count++;

    glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
        0,
        _in_texture_array->cumul_width, _in_texture_array->cumul_height, 0,
        width, height, 1,
        GL_RGBA, GL_UNSIGNED_BYTE,
        data
    );

    _in_texture_array->cumul_width += width;

    stbi_image_free(data);

    return (1);
}

int mgeTextureArray2DFree(TextureArray2D* _in_texture_array)
{
    if (!_in_texture_array)
    {
        printf("%s(%s) : Texture Array is NULL.\n", __func__, __FILE__);
        return (0);
    }

    for (int i = 0; i < _in_texture_array->quads_count; i++)
    {
        free(_in_texture_array->quads[i]->path);
        free( _in_texture_array->quads[i]);
    }

    free ( _in_texture_array->quads );

    return (1);
}