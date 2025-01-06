#ifndef H_MGE_BATCH
#define H_MGE_BATCH

#include "common.h"
#include "mesh.h"
#include "shader.h"

typedef struct MGEBatch MGEBatch;
typedef struct MGEBatchParam MGEBatchParam;

struct MGEBatch
{
    int batch_id; // The unique id of the MGEBatch
    int persistent; // Does the MGEBatch "flush" its data after each render call

    MGEArray mesh_cache; // Cache of all the meshes renderered by the MGEBatch

    MGEShader shader; // Shader used by the entire MGEBatch
    
    int mesh_max_count; // max number of mesh
    int vertex_max_count; // number of max verticies in the vbo
    int index_max_count; // number of max indicies in the ebo

    int mesh_count; // number of mesh in the MGEBatch
    int vertex_count; // number of verticies in the vbo
    int index_count; // number of indicies in the ebo

    unsigned int vao; // Vertex Array
    unsigned int vbo; // Vertex Buffer
    unsigned int ebo; // Element Buffer
    unsigned int dibo; // Draw Indirect Buffer

    unsigned int matrix_ssbo; // Matrix Shader Storage Buffer
    unsigned int texture_coords_ssbo; // Texture Coords Shader Storage Buffer

};

struct MGEBatchParam
{
    int persistent;
    MGEShader* shader;
};

MGEError mgeBatchInit(int _in_batch_id, MGEBatch** _out_batch);
MGEError mgeBatchFree(MGEBatch* _in_batch);

MGEError mgeBatchBindBuffers(MGEBatch* _in_batch);
MGEError mgeBatchUnbindBuffers();
MGEError mgeBatchResize(MGEBatch* _in_batch, int _in_mesh_count, int _in_vertex_count, int _in_index_count);

MGEError mgeBatchPushMesh(MGEBatch* _in_batch, MGEMesh* _in_mesh);

MGEError mgeBatchReset(MGEBatch* _in_batch, int _in_override);

#endif