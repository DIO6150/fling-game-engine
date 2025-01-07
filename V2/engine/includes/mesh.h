#ifndef H_FGE_MESH
#define H_FGE_MESH

#include "vertex.h"
#include "tex_array.h"
#include "texture_animation.h"

#include "cglm/cglm.h"
#include "cglm/mat4.h"

typedef struct FGEMesh FGEMesh;

struct FGEMesh
{
    unsigned int index;

    FGEVertex* vertices;
    int vertex_count;

    unsigned int* indices;
    int index_count;

    mat4 translation_matrix;
    mat4 rotation_matrix;
    mat4 scaling_matrix;

	char* tag;

    TextureAnimation animation;

    // move it in the batch

    int cache_batch_id;
    unsigned int cache_matrix_offset;
    unsigned int cache_texcoords_offset;

};

FGEMesh* FGEMeshNew (FGEVertex* in_vertices, unsigned int* in_indices,
                    int in_vertex_count, int in_index_count,
                    int in_tag,
                    TextureAnimation in_animation);
FGEError FGEMeshDelete (FGEMesh* in_mesh);

void FGEMeshTranslate (FGEMesh* in_mesh, vec3 in_translation);
void FGEMeshRotate (FGEMesh* in_mesh, float theta, vec3 in_axis);
void FGEMeshScale (FGEMesh* in_mesh, vec3 in_scale);

void FGEMeshSetScale (FGEMesh* in_mesh, vec3 in_scale);

void FGEMeshGetMatrix (FGEMesh* in_mesh, mat4 matrix);

#endif