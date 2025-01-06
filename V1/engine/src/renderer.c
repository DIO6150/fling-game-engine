#include <assert.h>
#include <string.h>

#include "glad/glad.h"

#include "core/renderer.h"

#ifndef MGE_PI
    #define MGE_PI 3.141592
#endif

/*   BATCH ORDER NODE   */

static void push_sort(BatchOrder** first, BatchOrder* second)
{
	if (!(*first)) { *first = second; return; }

	if ((*first)->order_id < second->order_id)
	{
		push_sort(&(*first)->next, second);
	}
	else
	{
		second->next = *first;
		*first = second;
	}
}

static void free_order(BatchOrder* first)
{
	if (!first) return;

	free_order(first->next);

	free(first);
}

/*   BATCH   */

int mgeBatchInit(int _in_batch_id, Batch** _out_batch)
{
    (*_out_batch) = (Batch*) malloc(sizeof(Batch));

    if (!*_out_batch)
    {
        printf("%s(%s) : Failed to init batch.\n", __func__, __FILE__);
        return (0);
    }

    (*_out_batch)->batch_id = _in_batch_id;

    (*_out_batch)->persistent = 0;
    (*_out_batch)->order_id = 0;
    (*_out_batch)->shader = NULL;

    (*_out_batch)->mesh_max_count = 0;
    (*_out_batch)->vertex_max_count = 0;
    (*_out_batch)->index_max_count = 0;

    (*_out_batch)->mesh_count = 0;
    (*_out_batch)->vertex_count = 0;
    (*_out_batch)->index_count = 0;

    glGenVertexArrays(1, &(*_out_batch)->vao);

    glGenBuffers(1, &(*_out_batch)->vbo);
    glGenBuffers(1, &(*_out_batch)->ebo);
    glGenBuffers(1, &(*_out_batch)->dibo);

    glGenBuffers(1, &(*_out_batch)->matrix_ssbo);
    glGenBuffers(1, &(*_out_batch)->texture_coords_ssbo);

    // since at this point, _out_batch is initialized, we don't do error handling
    mgeBatchBindBuffers(*_out_batch);

    glNamedBufferData((*_out_batch)->vbo, 1, NULL, GL_DYNAMIC_DRAW);
    glNamedBufferData((*_out_batch)->ebo, 1, NULL, GL_DYNAMIC_DRAW);
    glNamedBufferData((*_out_batch)->dibo, 1, NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, (*_out_batch)->matrix_ssbo);
    glNamedBufferData((*_out_batch)->matrix_ssbo, 1, NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, (*_out_batch)->texture_coords_ssbo);
    glNamedBufferData((*_out_batch)->texture_coords_ssbo, 1, NULL, GL_DYNAMIC_DRAW);

    // TODO : Allow custom vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    mgeBatchUnbindBuffers();

    return (1);
}

int mgeBatchFree(Batch* _in_batch)
{
    if (!_in_batch)
    {
        printf("%s(%s) : Batch was not initialized, aborting.\n", __func__, __FILE__);
        return (0);
    }

    glDeleteVertexArrays(1, &_in_batch->vao);

    glDeleteBuffers(1, &_in_batch->vbo);
    glDeleteBuffers(1, &_in_batch->ebo);
    glDeleteBuffers(1, &_in_batch->dibo);

    glDeleteBuffers(1, &_in_batch->matrix_ssbo);
    glDeleteBuffers(1, &_in_batch->texture_coords_ssbo);

    free(_in_batch);

    return (1);
}

int mgeBatchBindBuffers(Batch* _in_batch)
{
    if (!_in_batch)
    {
        printf("%s(%s) : Batch was not initialized.\n", __func__, __FILE__);
        return (0);
    }

    glBindVertexArray(_in_batch->vao);

    glBindBuffer(GL_ARRAY_BUFFER, _in_batch->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _in_batch->ebo);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, _in_batch->dibo);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _in_batch->matrix_ssbo);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _in_batch->texture_coords_ssbo);

    return (1);
}

int mgeBatchUnbindBuffers()
{
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

    return (1);
}

int mgeBatchResize(Batch* _in_batch, int _in_mesh_count, int _in_vertex_count, int _in_index_count)
{
    if (!_in_batch)
    {
        printf("%s(%s) : Batch was not initialized\n", __func__, __FILE__);
        return (0);
    }

    void* old_vbo_data = NULL;
    void* old_ebo_data = NULL;
    void* old_dibo_data = NULL;
    void* old_matrix_ssbo_data = NULL;
    void* old_texcoords_ssbo_data = NULL;

    // TODO : There is actually undefined behavior here,
    // If I get old data with old size into an old size container then fuck me I guess ?
    // Because the data packed next to it is not mine and therefore i'm shit

    if (_in_batch->mesh_count != 0)
    {

        old_vbo_data = glMapNamedBuffer(_in_batch->vbo, GL_READ_WRITE);
        old_ebo_data = glMapNamedBuffer(_in_batch->ebo, GL_READ_WRITE);
        old_dibo_data = glMapNamedBuffer(_in_batch->dibo, GL_READ_WRITE);
        old_matrix_ssbo_data = glMapNamedBuffer(_in_batch->matrix_ssbo, GL_READ_WRITE);
        old_texcoords_ssbo_data = glMapNamedBuffer(_in_batch->texture_coords_ssbo, GL_READ_WRITE);

        if (!old_vbo_data || !old_ebo_data || !old_dibo_data || !old_matrix_ssbo_data || !old_texcoords_ssbo_data)
        {
            if (old_vbo_data)
                glUnmapNamedBuffer(_in_batch->vbo);

            if (old_ebo_data)
                glUnmapNamedBuffer(_in_batch->ebo);

            if (old_dibo_data)
                glUnmapNamedBuffer(_in_batch->dibo);

            if (old_matrix_ssbo_data)
                glUnmapNamedBuffer(_in_batch->matrix_ssbo);

            if (old_texcoords_ssbo_data)
                glUnmapNamedBuffer(_in_batch->texture_coords_ssbo);

            printf("%s(%s) : Error when mapping OpenGL buffer.\n", __func__, __FILE__);
            return (0);
        }     
    }

    _in_batch->mesh_max_count += _in_mesh_count;
    _in_batch->vertex_max_count += _in_vertex_count;
    _in_batch->index_max_count += _in_index_count;

    glNamedBufferData(_in_batch->vbo, _in_batch->vertex_max_count * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);
    glNamedBufferSubData(_in_batch->vbo, 0, _in_batch->vertex_count * sizeof(Vertex), old_vbo_data);

    glNamedBufferData(_in_batch->ebo, _in_batch->index_max_count * sizeof(int), NULL, GL_DYNAMIC_DRAW);
    glNamedBufferSubData(_in_batch->ebo, 0, _in_batch->index_count * sizeof(int), old_ebo_data);

    glNamedBufferData(_in_batch->dibo, _in_batch->mesh_max_count * sizeof(DrawCommand), NULL, GL_DYNAMIC_DRAW);
    glNamedBufferSubData(_in_batch->dibo, 0, _in_batch->mesh_count * sizeof(DrawCommand), old_dibo_data);

    glNamedBufferData(_in_batch->matrix_ssbo, _in_batch->mesh_max_count * sizeof(mat4), NULL, GL_DYNAMIC_DRAW);
    glNamedBufferSubData(_in_batch->matrix_ssbo, 0, _in_batch->mesh_count * sizeof(mat4), old_matrix_ssbo_data);

    glNamedBufferData(_in_batch->texture_coords_ssbo, _in_batch->mesh_max_count * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
    glNamedBufferSubData(_in_batch->texture_coords_ssbo, 0, _in_batch->mesh_count * sizeof(vec4), old_texcoords_ssbo_data);

    return (1);
}

int mgeBatchPushMesh(Batch* _in_batch, Mesh* _in_mesh)
{
    if (!_in_batch)
    {
        printf("%s(%s) : Batch is NULL.\n", __func__, __FILE__);
        return (0);
    }

    if (!_in_mesh)
    {
        printf("%s(%s) : Mesh is NULL.\n", __func__, __FILE__);
        return (0);
    }

    if (_in_batch->vertex_count + _in_mesh->vertex_count >= _in_batch->vertex_max_count || 
        _in_batch->index_count + _in_mesh->index_count >= _in_batch->index_max_count)

    {
        if (!mgeBatchResize(_in_batch, 1, _in_mesh->vertex_count, _in_mesh->index_count))
        {
            printf("%s(%s) : Couldn't resize, aborting.\n", __func__, __FILE__);
            printf("Mesh Count : %d / %d\n", _in_batch->mesh_count, _in_batch->mesh_max_count);
            return (0);
        }
    }

    _in_mesh->cache_matrix_offset = _in_batch->mesh_count * sizeof(mat4);
    _in_mesh->cache_texcoords_offset = _in_batch->mesh_count * sizeof(vec4);

    // TODO : Take into account instance rendering
    DrawCommand command;

    command.count = _in_mesh->index_count;
    command.instance_count = 1;
    command.first_index = _in_batch->index_count;
    command.base_vertex = _in_batch->vertex_count;
    command.base_instance = 0;

    // Vertex Data
    glNamedBufferSubData(_in_batch->vbo, _in_batch->vertex_count * sizeof(Vertex), _in_mesh->vertex_count * sizeof(Vertex), _in_mesh->vertices);

    // Index Data
    glNamedBufferSubData(_in_batch->ebo, _in_batch->index_count * sizeof(int), _in_mesh->index_count * sizeof(int), _in_mesh->indices);

    // Draw Command Data
    glNamedBufferSubData(_in_batch->dibo, _in_batch->mesh_count * sizeof(DrawCommand), sizeof(DrawCommand), &command);

    // Matrix Data
    mat4 model_matrix;

    mgeMeshGetMatrix(_in_mesh, model_matrix);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _in_batch->matrix_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _in_batch->matrix_ssbo);
    glNamedBufferSubData(_in_batch->matrix_ssbo, _in_batch->mesh_count * sizeof(mat4), sizeof(mat4), model_matrix);

    // Texture Coords Ratio Data

    TextureQuad* quad = mgeAnimationGetQuad(&(_in_mesh->animation));

    float width =  (float) (quad->w ) / MGE_TEXTURE_ARRAY_2D_SIZE;
    float height = (float) (quad->h ) / MGE_TEXTURE_ARRAY_2D_SIZE;

    float x = (float) (quad->x ) / MGE_TEXTURE_ARRAY_2D_SIZE;
    float y = (float) (quad->y ) / MGE_TEXTURE_ARRAY_2D_SIZE;

    vec4 coords_ratio = { x, y,  width, height };

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _in_batch->texture_coords_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _in_batch->texture_coords_ssbo);
    glNamedBufferSubData(_in_batch->texture_coords_ssbo, _in_batch->mesh_count * sizeof(vec4), sizeof(vec4), coords_ratio);

    _in_batch->mesh_count++;
    _in_batch->vertex_count += _in_mesh->vertex_count;
    _in_batch->index_count += _in_mesh->index_count;

    return (1);
}

int mgeBatchReset(Batch* _in_batch, int _in_override)
{
    if (!_in_batch)
    {
        printf("%s(%s) : Batch was not initialized.\n", __func__, __FILE__);
        return (0);
    }

    if (!_in_batch->persistent || _in_override)
    {
        _in_batch->mesh_count = 0;
        _in_batch->vertex_count = 0;
        _in_batch->index_count = 0;
    }

    return (1);
}

/*   RENDERER   */

int mgeRendererInit(Renderer** _out_renderer, ViewType _in_type, vec2 _in_window_size)
{
    *_out_renderer = malloc(sizeof(Renderer));

    if (!(*_out_renderer))
    {
        printf("%s(%s) : Renderer couldn't be initialized.\n", __func__, __FILE__);
        return (0);
    }

    for (int i = 0; i < MGE_BATCH_MAX_COUNT; i++)
	{
        (*_out_renderer)->batches[i] = NULL;
	}

	(*_out_renderer)->order_cache = NULL;
    (*_out_renderer)->batch_count = 0;

    (*_out_renderer)->vtype = _in_type;

	if (_in_type == MGE_ORTHOGRAPHIC)
	{
        float aspect_ratio = _in_window_size[0] / _in_window_size[1];

        float n = 1.0f;

		glm_ortho( -5.f * n, +5.f * n, -5.f * n / aspect_ratio, +5.f * n / aspect_ratio, .1f, 100.0f, (*_out_renderer)->projection);
	}
	else
	{
    	// rounded value of 45° in radians
    	glm_perspective(0.785398f, 1.0f, 0.1f, 100.0f, (*_out_renderer)->projection);
	}

    (*_out_renderer)->zoom = 45.0f;

    if (!mgeCameraInit(&((*_out_renderer)->camera), (vec3) { 0.0f, 0.0f, 1.0f }))
    {
        free(*_out_renderer);
        printf("%s(%s) : Camera failed to initialize, aborting.\n", __func__, __FILE__);
        return (0);
    }

    if (!mgeTextureArray2DInit(&(*_out_renderer)->atlas))
    {
        mgeRendererFree(*_out_renderer);
        printf("%s(%s) : Texture Atlas failed to initialize, aborting.\n", __func__, __FILE__);
        return (0);
    }

    (*_out_renderer)->animations = malloc( sizeof (TextureAnimation*) * 16);
    (*_out_renderer)->animation_count = 0;
    (*_out_renderer)->animation_max_count = 16;


    return (1);
}

int mgeRendererFree(Renderer* _in_renderer)
{
    if (!_in_renderer)
    {
        printf("%s(%s) : Renderer isn't initialized.\n", __func__, __FILE__);
        return (0);
    }

    for (int i = 0; i < _in_renderer->batch_count; i++)
    {
        mgeBatchFree(_in_renderer->batches[i]);
    }

    free(_in_renderer->animations);

    mgeCameraFree(_in_renderer->camera);

	free_order(_in_renderer->order_cache);

    free(_in_renderer);

    return (1);
}

int mgeRendererOrthoResize(Renderer* _in_renderer, float unit, vec2 _in_window_size)
{
    float aspect_ratio = _in_window_size[0] / _in_window_size[1];
    float n = unit / 10.0f;
    glm_ortho( -5.f * n, +5.f * n, -5.f * n / aspect_ratio, +5.f * n / aspect_ratio, .1f, 100.0f, _in_renderer->projection);

    return (1);
}

int mgeRendererOrthoResizeOn(Renderer* _in_renderer, float unit, vec2 offset, vec2 _in_window_size)
{
    float aspect_ratio = _in_window_size[0] / _in_window_size[1];
    float n = unit / 10.0f;
    glm_ortho( -5.f * n + offset[0], +5.f * n + offset[1], -5.f * n / aspect_ratio + offset[0], +5.f * n / aspect_ratio + offset[1], .1f, 100.0f, _in_renderer->projection);

    return (1);
}

int mgeRendererZoom(Renderer* _in_renderer, float zoom)
{
    if (!_in_renderer)
    {
        printf("%s(%s) : Renderer is NULL.\n", __func__, __FILE__);
        return (0);
    }

    _in_renderer->zoom = _in_renderer->zoom * zoom;

    if (_in_renderer->vtype == MGE_PERSPECTIVE)
        glm_perspective(_in_renderer->zoom * MGE_PI / 180.0f, 1.0f, 0.1f, 100.0f, _in_renderer->projection);
    
    if (_in_renderer->vtype == MGE_ORTHOGRAPHIC)
        return (1);

    return (1);
}

int mgeRendererSetZoom(Renderer* _in_renderer, float zoom)
{
    if (!_in_renderer)
    {
        printf("%s(%s) : Renderer is NULL.\n", __func__, __FILE__);
        return (0);
    }

    _in_renderer->zoom = zoom;

    if (_in_renderer->vtype == MGE_PERSPECTIVE)
        glm_perspective(zoom * MGE_PI / 180.0f, 1.0f, 0.1f, 100.0f, _in_renderer->projection);

    if (_in_renderer->vtype == MGE_ORTHOGRAPHIC)
        return (1);

    return (1);
}

int mgeRendererSetBatchStatic(Renderer* _in_renderer, int _in_batch_id, int _in_persist)
{
    if (!_in_renderer)
    {
        printf("%s(%s) : Renderer is NULL.\n", __func__, __FILE__);
        return (0);
    }

    if (_in_batch_id < 0 || _in_batch_id >= MGE_BATCH_MAX_COUNT)
    {
        printf("%s(%s) : Invalid index : %d.\n", __func__, __FILE__, _in_batch_id);
        return (0);
    }

    if (!_in_renderer->batches[_in_batch_id])
    {
        if (!mgeBatchInit(_in_batch_id, _in_renderer->batches +_in_batch_id))
        {
            printf("%s(%s) : Couldn't create a new batch, aborting.\n", __func__, __FILE__);
            return (0);
        }

        _in_renderer->batch_count++;
    }

    _in_renderer->batches[_in_batch_id]->persistent = _in_persist;

    return (1);
}

int mgeRendererSetBatchShader(Renderer* _in_renderer, int _in_batch_id, Shader* _in_shader)
{
    if (!_in_renderer)
    {
        printf("%s(%s) : Renderer is NULL.\n", __func__, __FILE__);
        return (0);
    }

    if (!_in_shader)
    {
        printf("%s(%s) : Shader is NULL.\n", __func__, __FILE__);
        return (0);
    }

    if (_in_batch_id < 0 || _in_batch_id >= MGE_BATCH_MAX_COUNT)
    {
        printf("%s(%s) : Invalid index : %d.\n", __func__, __FILE__, _in_batch_id);
        return (0);
    }

    if (!_in_renderer->batches[_in_batch_id])
    {
        if (!mgeBatchInit(_in_batch_id, _in_renderer->batches +_in_batch_id))
        {
            printf("%s(%s) : Couldn't create a new batch, aborting.\n", __func__, __FILE__);
            return (0);
        }

        _in_renderer->batch_count++;
    }

    _in_renderer->batches[_in_batch_id]->shader = _in_shader;

    return (1);
}

int mgeRendererSetBatchRenderingOrder(Renderer* _in_renderer, int _in_batch_id, unsigned int _in_order)
{
	if (!_in_renderer)
    {
        printf("%s(%s) : Renderer is NULL.\n", __func__, __FILE__);
        return (0);
    }

    if (_in_batch_id < 0 || _in_batch_id >= MGE_BATCH_MAX_COUNT)
    {
        printf("%s(%s) : Invalid index : %d.\n", __func__, __FILE__, _in_batch_id);
        return (0);
    }

	if (!_in_renderer->batches[_in_batch_id])
    {
        if (!mgeBatchInit(_in_batch_id, _in_renderer->batches +_in_batch_id))
        {
            printf("%s(%s) : Couldn't create a new batch, aborting.\n", __func__, __FILE__);
            return (0);
        }

        _in_renderer->batch_count++;
    }

	_in_renderer->batches[_in_batch_id]->order_id = _in_order;

	return (1);
}

int mgeRendererPushMesh(Renderer* _in_renderer, int _in_batch_id, Mesh* _in_mesh)
{
    if (!_in_renderer)
    {
        printf("%s(%s) : Renderer is NULL.\n", __func__, __FILE__);
        return (0);
    }

    if (!_in_mesh)
    {
        printf("%s(%s) : Mesh is NULL.\n", __func__, __FILE__);
        return (0);
    }

    if (_in_batch_id < 0 || _in_batch_id >= MGE_BATCH_MAX_COUNT)
    {
        printf("%s(%s) : Invalid index : %d.\n", __func__, __FILE__, _in_batch_id);
        return (0);
    }

    if (_in_batch_id >= _in_renderer->batch_count)
    {
        if (!_in_renderer->batches[_in_batch_id])
        {
            if (!mgeBatchInit(_in_batch_id, _in_renderer->batches +_in_batch_id))
            {
                printf("%s(%s) : Couldn't create a new batch, aborting.\n", __func__, __FILE__);
                return (0);
            }

            _in_renderer->batch_count++;
        }
    }

    _in_mesh->cache_batch_id = _in_batch_id;

    Batch* batch = _in_renderer->batches[_in_batch_id];

    mgeBatchPushMesh(batch, _in_mesh);

    return (1);
}

int mgeRendererUpdateMeshMatrix(Renderer* _in_renderer, Mesh* _in_mesh)
{
    if (!_in_renderer)
    {
        printf("%s(%s) : Renderer is NULL.\n", __func__, __FILE__);
        return (0);
    }

    if (!_in_mesh)
    {
        printf("%s(%s) : Mesh is NULL.\n", __func__, __FILE__);
        return (0);
    }

    if (_in_mesh->cache_batch_id < 0 || _in_mesh->cache_batch_id >= MGE_BATCH_MAX_COUNT)
    {
        printf("%s(%s) : Invalid index : %d.\n", __func__, __FILE__, _in_mesh->cache_batch_id);
        return (0);
    }

    Batch* batch = _in_renderer->batches[_in_mesh->cache_batch_id];

    if (!batch->mesh_count) return (1);

    // Matrix Data
    mat4 model_matrix;

    mgeMeshGetMatrix(_in_mesh, model_matrix);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, batch->matrix_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, batch->matrix_ssbo);
    glNamedBufferSubData(batch->matrix_ssbo,  _in_mesh->cache_matrix_offset, sizeof(mat4), model_matrix);

    return (1);
}

int mgeRendererUpdateMeshTextureCoordinates(Renderer* _in_renderer, Mesh* _in_mesh)
{
    if (!_in_renderer)
    {
        printf("%s(%s) : Renderer is NULL.\n", __func__, __FILE__);
        return (0);
    }

    if (!_in_mesh)
    {
        printf("%s(%s) : Mesh is NULL.\n", __func__, __FILE__);
        return (0);
    }

    if (_in_mesh->cache_batch_id < 0 || _in_mesh->cache_batch_id >= MGE_BATCH_MAX_COUNT)
    {
        printf("%s(%s) : Invalid index : %d.\n", __func__, __FILE__, _in_mesh->cache_batch_id);
        return (0);
    }

    Batch* batch = _in_renderer->batches[_in_mesh->cache_batch_id];

    if (!batch->mesh_count) return (1);

    // Texture Coordinates Data

    TextureQuad* quad = mgeAnimationGetQuad(&(_in_mesh->animation));

    float width =  (float) (quad->w ) / MGE_TEXTURE_ARRAY_2D_SIZE;
    float height = (float) (quad->h ) / MGE_TEXTURE_ARRAY_2D_SIZE;

    float x = (float) (quad->x ) / MGE_TEXTURE_ARRAY_2D_SIZE;
    float y = (float) (quad->y ) / MGE_TEXTURE_ARRAY_2D_SIZE;

    vec4 coords_ratio = { x, y,  width, height };

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, batch->texture_coords_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, batch->texture_coords_ssbo);
    glNamedBufferSubData(batch->texture_coords_ssbo, _in_mesh->cache_texcoords_offset, sizeof(vec4), coords_ratio);

    return (1);
}

int mgeRendererCreateAnimationTexture(Renderer* _in_renderer, TextureAnimation** _out_animation, char* _in_name)
{
    mgeAnimationCreate(_out_animation, _in_renderer->atlas, _in_name);

    assert(*_out_animation);

    if (_in_renderer->animation_count >= _in_renderer->animation_max_count)
    {
        _in_renderer->animation_max_count *= 2;
        TextureAnimation** array = realloc(_in_renderer->animations, sizeof (TextureAnimation*) * _in_renderer->animation_max_count);
        assert(array);

        _in_renderer->animations = array;
    }

    _in_renderer->animations[_in_renderer->animation_count] = (*_out_animation);
    _in_renderer->animation_count ++;

    return (1);
}

TextureAnimation* mgeRendererGetAnimationTexture(Renderer* _in_renderer, char* _in_name)
{
    for (int i = 0; i < _in_renderer->animation_count; i ++)
    {
        if (!strcmp(_in_name, _in_renderer->animations[i]->name))
        {
            return (_in_renderer->animations[i]);
        }
    }

    return (NULL);
}

int mgeRendererClearBatch(Renderer* _in_renderer, int _in_batch_id)
{
	if (!_in_renderer)
    {
        printf("%s(%s) : Renderer is NULL.\n", __func__, __FILE__);
        return (0);
    }

	if (_in_batch_id >= _in_renderer->batch_count)
    {
		printf("%s(%s) : Invalid Batch Index.\n", __func__, __FILE__);
        return (0);
	}

	return (mgeBatchReset(_in_renderer->batches[_in_batch_id], 1));
}

static void __attribute__((unused)) print_batch_order(BatchOrder* order)
{
	static int n_call = 0;

	if (!order) 
	{
		n_call = 0;
		return;
	}

	n_call ++;

	printf ("(%x %d [%d])", order->order_id, order->index, n_call);
	if (!order->next) printf("\n");
	else printf(" -> ");

	print_batch_order(order->next);
}

int mgeRendererUpdateBatchOrderCache(Renderer* _in_renderer)
{
	if (!_in_renderer)
    {
        printf("%s(%s) : Renderer is NULL\n", __func__, __FILE__);
        return (0);
    }

	free_order(_in_renderer->order_cache);

	for (int i = 0; i < _in_renderer->batch_count; i++)
	{
		BatchOrder* node = (BatchOrder*) malloc(sizeof(BatchOrder));

		node->order_id =_in_renderer->batches[i]->order_id;
		node->index = i;
		node->next = NULL;

		push_sort(&_in_renderer->order_cache, node);
	}

	printf("\n");

	return (1);
}

int mgeRender(Renderer* _in_renderer)
{
    if (!_in_renderer)
    {
        printf("%s(%s) : Renderer is NULL\n", __func__, __FILE__);
        return (0);
    }

    mat4 view;
    if (!mgeCameraGetView(_in_renderer->camera, view))
    {
        printf("%s(%s) : Couldn't create camera view, aborting.\n", __func__, __FILE__);
        return (0);
    }

	for (int i = 0; i < _in_renderer->batch_count; i++)
	{
		if (!_in_renderer->batches[i])
        {
            printf("%s(%s) : Batch [%d] is NULL.\n", __func__, __FILE__, i);
            continue;
        }

        if (!mgeBatchBindBuffers(_in_renderer->batches[i]))
        {
            printf("%s(%s) : Error when binding buffers of batch [%d]\n", __func__, __FILE__, i);
            continue;
        }

        if (!_in_renderer->batches[i]->shader)
        {
            printf("%s(%s) : Shader of batch [%d] is NULL.\n", __func__, __FILE__, i);
            continue;
        }

        mgeShaderUse(_in_renderer->batches[i]->shader);
        int location;

        location = glGetUniformLocation(_in_renderer->batches[i]->shader->program, "uProjection");
        glUniformMatrix4fv(location, 1, GL_FALSE, *_in_renderer->projection);

        location = glGetUniformLocation(_in_renderer->batches[i]->shader->program, "uView");
        glUniformMatrix4fv(location, 1, GL_FALSE, *view);

		location = glGetUniformLocation(_in_renderer->batches[i]->shader->program, "uTileSet");
        glUniform1i(location, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, _in_renderer->atlas->object);

        /*
        // for debugging
        Vertex* __attribute__((unused)) vbo_ptr = (Vertex*) glMapNamedBuffer(_in_renderer->batches[i]->vbo, GL_READ_ONLY);
        unsigned int* __attribute__((unused)) ebo_ptr = (unsigned int*) glMapNamedBuffer(_in_renderer->batches[i]->ebo, GL_READ_ONLY);
        DrawCommand* __attribute__((unused)) dibo_ptr = (DrawCommand*) glMapNamedBuffer(_in_renderer->batches[i]->dibo, GL_READ_ONLY);
        
        glUnmapNamedBuffer(_in_renderer->batches[i]->vbo);
        glUnmapNamedBuffer(_in_renderer->batches[i]->ebo);
        glUnmapNamedBuffer(_in_renderer->batches[i]->dibo);
        */
       
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, NULL, _in_renderer->batches[i]->mesh_count, 0);

        mgeShaderCancel();

        mgeBatchUnbindBuffers();

        mgeBatchReset(_in_renderer->batches[i], 0);
	}

    return (1);
}