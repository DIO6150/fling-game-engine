#ifndef H_MGE_RENDERER
#define H_MGE_RENDERER

#include "core/camera.h"
#include "core/mesh.h"
#include "core/shader.h"
#include "core/texture_animation.h"
#include "core/texture.h"

typedef struct BatchOrder BatchOrder;



/*   DRAW COMMANDS   */


// Data
typedef struct
{
    unsigned int count;
    unsigned int instance_count;
    unsigned int first_index;
    int base_vertex;
    unsigned int base_instance;
} DrawCommand;



/*   BATCH   */


// Data
typedef struct
{
    int batch_id; // The unique id of the batch

    int persistent; // Does the batch "flush" its data after each render call
	unsigned int order_id; // Lower Id means last to draw

    Shader* shader; // Shader used by the entire batch
    
    int mesh_max_count; // max number of mesh
    int vertex_max_count; // number of max verticies in the vbo
    int index_max_count; // number of max indicies in the ebo

    int mesh_count; // number of mesh in the batch
    int vertex_count; // number of verticies in the vbo
    int index_count; // number of indicies in the ebo

    unsigned int vao; // Vertex Array
    unsigned int vbo; // Vertex Buffer
    unsigned int ebo; // Element Buffer
    unsigned int dibo; // Draw Indirect Buffer

    unsigned int matrix_ssbo; // Matrix Shader Storage Buffer
    unsigned int texture_coords_ssbo; // Texture Coords Shader Storage Buffer

    Mesh** mesh_cache;
	
} Batch;

// Functions
int mgeBatchInit(int _in_batch_id, Batch** _out_batch);
int mgeBatchFree(Batch* _in_batch);

int mgeBatchBindBuffers(Batch* _in_batch);
int mgeBatchUnbindBuffers();
int mgeBatchResize(Batch* _in_batch, int _in_mesh_count, int _in_vertex_count, int _in_index_count);

int mgeBatchPushMesh(Batch* _in_batch, Mesh* _in_mesh);

int mgeBatchReset(Batch* _in_batch, int _in_override);


/*   RENDERER   */

// TODO : its gonna be dynamic
#define MGE_BATCH_MAX_COUNT 16

typedef enum
{
	MGE_PERSPECTIVE, MGE_ORTHOGRAPHIC
} ViewType;

// Data
typedef struct
{
    Batch* batches[MGE_BATCH_MAX_COUNT];
	BatchOrder* order_cache;
    int batch_count;

    mat4 projection;

    Camera* camera;

    TextureArray2D* atlas;
    TextureAnimation** animations;
    int animation_count;
    int animation_max_count;

    ViewType vtype;
    float zoom;
} Renderer;

struct BatchOrder
{
	unsigned int order_id;
	int index;
	BatchOrder* next;
};

// Functions
int mgeRendererInit(Renderer** _out_renderer, ViewType _in_type, vec2 _in_window_size);
int mgeRendererFree(Renderer* _in_renderer);

int mgeRendererOrthoResize(Renderer* _in_renderer, float unit, vec2 _in_window_size);
int mgeRendererOrthoResizeOn(Renderer* _in_renderer, float unit, vec2 offset, vec2 _in_window_size);

int mgeRendererZoom(Renderer* _in_renderer, float zoom);
int mgeRendererSetZoom(Renderer* _in_renderer, float zoom);

int mgeRendererSetBatchStatic(Renderer* _in_renderer, int _in_batch_id, int _in_persist);
int mgeRendererSetBatchShader(Renderer* _in_renderer, int _in_batch_id, Shader* _in_shader);
int mgeRendererSetBatchRenderingOrder(Renderer* _in_renderer, int _in_batch_id, unsigned int _in_order);

int mgeRendererPushMesh(Renderer* _in_renderer, int _in_batch_id, Mesh* _in_mesh);

// TODO : now the mesh caches the ID but it would be better to cache the meshes in the batch (there would be many application to know what meshes are currently indide a batch)
int mgeRendererUpdateMeshMatrix(Renderer* _in_renderer, Mesh* _in_mesh);
int mgeRendererUpdateMeshTextureCoordinates(Renderer* _in_renderer, Mesh* _in_mesh);

int mgeRendererCreateAnimationTexture(Renderer* _in_renderer, TextureAnimation** _out_animation, char* _in_name);
TextureAnimation* mgeRendererGetAnimationTexture(Renderer* _in_renderer, char* _in_name);

int mgeRendererClearBatch(Renderer* _in_renderer, int _in_batch_id);

int mgeRendererUpdateBatchOrderCache(Renderer* _in_renderer);

int mgeRender(Renderer* _in_renderer);

#endif