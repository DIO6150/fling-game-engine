#ifndef H_MGE_MESH
#define H_MGE_MESH

#include "vertex.h"
#include "texture_array_2D.h"
#include "texture_animation.h"

#include "cglm/cglm.h"
#include "cglm/mat4.h"

typedef struct MGEMesh MGEMesh;

struct MGEMesh
{
    unsigned int index;

    Vertex* vertices;
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

MGEMesh* mgeMeshInit (unsigned int _in_index, Vertex* _in_vertices, unsigned int* _in_indices, int _in_vertex_count,
                        int _in_index_count, int _in_tag, TextureAnimation _in_animation);
MGEError mgeMeshFree (MGEMesh* _in_mesh);

int mgeMeshChangeColor (MGEMesh* _in_mesh, vec3 _in_color);

void mgeMeshTranslate (MGEMesh* _in_mesh, vec3 _in_translation);
void mgeMeshRotate (MGEMesh* _in_mesh, float theta, vec3 _in_axis);
void mgeMeshScale (MGEMesh* _in_mesh, vec3 _in_scale);

void mgeMeshSetScale (MGEMesh* _in_mesh, vec3 _in_scale);

void mgeMeshGetMatrix (MGEMesh* _in_mesh, mat4 matrix);

#endif