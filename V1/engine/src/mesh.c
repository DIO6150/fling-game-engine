
#include <stdlib.h>
#include <string.h>

#include "core/mesh.h"


int mgeMeshInit(unsigned int _in_index, Vertex* _in_vertices, unsigned int* _in_indices, int _in_vertex_count, int _in_index_count, 
                Mesh** _out_mesh, char* _in_tag, TextureAnimation _in_animation)
{
    *_out_mesh = (Mesh*) malloc(sizeof(Mesh));

    (*_out_mesh)->index = _in_index;

    (*_out_mesh)->vertices = (Vertex*) malloc(sizeof(Vertex) * _in_vertex_count);

    if (!(*_out_mesh)->vertices)
    {
        printf("%s(%s) : Couldn't initialize vertex memory.\n", __func__, __FILE__);
        return (0);
    }

    (*_out_mesh)->indices = (unsigned int*) malloc(sizeof(int) * _in_index_count);

    if (!(*_out_mesh)->indices)
    {
        free((*_out_mesh)->vertices);
        free((*_out_mesh));
        printf("%s(%s) : Couldn't initialize index memory.\n", __func__, __FILE__);
        return (0);
    }

    memcpy((*_out_mesh)->vertices, _in_vertices, _in_vertex_count * sizeof(Vertex));
    memcpy((*_out_mesh)->indices, _in_indices, _in_index_count * sizeof(int));

    (*_out_mesh)->vertex_count = _in_vertex_count;
    (*_out_mesh)->index_count = _in_index_count;

    glm_translate_make((*_out_mesh)->translation_matrix, (vec3) {0.0f, 0.0f, 0.0f});
    glm_rotate_make((*_out_mesh)->rotation_matrix, 0.0f, (vec3) {0.0f, 0.0f, 0.0f});
    glm_scale_make((*_out_mesh)->scaling_matrix, (vec3) {1.0f, 1.0f, 1.0f});

	(*_out_mesh)->tag = malloc(sizeof(char) * ( strlen(_in_tag) + 1) );
	strcpy((*_out_mesh)->tag, _in_tag);

    (*_out_mesh)->animation = _in_animation;


    (*_out_mesh)->cache_batch_id = -1;
    (*_out_mesh)->cache_matrix_offset = 0;
    (*_out_mesh)->cache_texcoords_offset = 0;
    
    return (1);
}

int mgeMeshFree(Mesh* _in_mesh)
{
    if (!_in_mesh)
    {
        printf("%s(%s) : Mesh is NULL.\n", __func__, __FILE__);
        return (0);
    }

    free(_in_mesh->vertices);
    free(_in_mesh->indices);
	free(_in_mesh->tag);
    free(_in_mesh);

    return (1);
}

int mgeMeshChangeColor(Mesh* _in_mesh, vec3 _in_color)
{
	if (!_in_mesh)
    {
        printf("%s(%s) : Mesh is NULL.\n", __func__, __FILE__);
        return (0);
    }

	for (int i = 0; i < _in_mesh->vertex_count; i++)
	{
		_in_mesh->vertices[i].r = _in_color[0];
		_in_mesh->vertices[i].g = _in_color[1];
		_in_mesh->vertices[i].b = _in_color[2];
	}

	return (1);
}

int mgeMeshTranslate(Mesh* _in_mesh, vec3 _in_translation)
{
    if (!_in_mesh)
    {
        printf("%s(%s) : Mesh is NULL.\n", __func__, __FILE__);
        return (0);
    }

    glm_translate(_in_mesh->translation_matrix, _in_translation);

    return (1);
}

int mgeMeshRotate(Mesh* _in_mesh, float theta, vec3 _in_axis)
{
    if (!_in_mesh)
    {
        printf("%s(%s) : Mesh is NULL.\n", __func__, __FILE__);
        return (0);
    }

    glm_rotate(_in_mesh->rotation_matrix, theta, _in_axis);

    return (1);
}

int mgeMeshScale(Mesh* _in_mesh, vec3 _in_scale)
{
    if (!_in_mesh)
    {
        printf("%s(%s) : Mesh is NULL.\n", __func__, __FILE__);
        return (0);
    }

    glm_scale(_in_mesh->scaling_matrix, _in_scale);

    return (1);
}

int mgeMeshSetScale(Mesh* _in_mesh, vec3 _in_scale)
{
    if (!_in_mesh)
    {
        printf("%s(%s) : Mesh is NULL.\n", __func__, __FILE__);
        return (0);
    }

    glm_scale_make(_in_mesh->scaling_matrix, _in_scale);

    return (1);
}

int mgeMeshGetMatrix(Mesh* _in_mesh, mat4 matrix)
{
    if (!_in_mesh)
    {
        printf("%s(%s) : Mesh is NULL.\n", __func__, __FILE__);
        return (0);
    }

    glm_mat4_mul(_in_mesh->translation_matrix, _in_mesh->rotation_matrix, matrix);
    glm_mat4_mul(matrix, _in_mesh->scaling_matrix, matrix);

    return (1);
}