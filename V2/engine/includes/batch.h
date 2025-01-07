#ifndef HEADER_FGE_BATCH
#define HEADER_FGE_BATCH

#include "common/common.h"
#include "common/vector_impl.h"
#include "mesh.h"
#include "shader.h"

typedef struct FGEBatch FGEBatch;
typedef struct FGEBatchParam FGEBatchParam;

struct FGEBatch
{
    int batch_id; // The unique id of the FGEBatch
    int persistent; // Does the FGEBatch "flush" its data after each render call

    FGEMeshVector mesh_cache; // Cache of all the meshes renderered by the FGEBatch

    FGEShader shader; // Shader used by the entire FGEBatch
    
    int mesh_max_count; // max number of mesh
    int vertex_max_count; // number of max verticies in the vbo
    int index_max_count; // number of max indicies in the ebo

    int mesh_count; // number of mesh in the FGEBatch
    int vertex_count; // number of verticies in the vbo
    int index_count; // number of indicies in the ebo

    unsigned int vao; // Vertex Array
    unsigned int vbo; // Vertex Buffer
    unsigned int ebo; // Element Buffer
    unsigned int dibo; // Draw Indirect Buffer

    unsigned int matrix_ssbo; // Matrix Shader Storage Buffer
    unsigned int texture_coords_ssbo; // Texture Coords Shader Storage Buffer

};

struct FGEBatchParam
{
    int persistent;
    FGEShader* shader;
};

FGEBatch* FGEBatchNew (int in_batch_id);
FGEError FGEBatchDelete (FGEBatch* in_batch);

FGEError FGEBatchBindBuffers(FGEBatch* in_batch);
FGEError FGEBatchUnbindBuffers();
FGEError FGEBatchResize(FGEBatch* in_batch, int in_mesh_count, int in_vertex_count, int in_index_count);

FGEError FGEBatchPushMesh(FGEBatch* in_batch, FGEMesh* in_mesh);

FGEError FGEBatchReset(FGEBatch* in_batch, int in_override);

#endif