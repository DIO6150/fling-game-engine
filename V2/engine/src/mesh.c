#include <string.h>

#include "mesh.h"

#include "cglm/cglm.h"

FGEMesh* FGEMeshNew (FGEVertex* in_vertices, unsigned int* in_indices,
                    int in_vertex_count, int in_index_count,
                    TextureAnimation in_animation)
{
    FGEMesh* mesh = (FGEMesh*) malloc (sizeof (FGEMesh));

    mesh->vertices = (FGEVertex*) malloc (sizeof (FGEVertex) * in_vertex_count);

    if (!mesh->vertices)
    {
        printf("%s(%s) : Couldn't initialize vertex memory.\n", __func__, __FILE__);
        return (NULL);
    }

    mesh->indices = (unsigned int*) malloc (sizeof (int) * in_index_count);

    if (!mesh->indices)
    {
        free (mesh->vertices);
        free (mesh);
        printf("%s(%s) : Couldn't initialize index memory.\n", __func__, __FILE__);
        return (NULL);
    }

    memcpy (mesh->vertices, in_vertices, in_vertex_count * sizeof (FGEVertex));
    memcpy (mesh->indices, in_indices, in_index_count * sizeof (int));

    mesh->vertex_count = in_vertex_count;
    mesh->index_count = in_index_count;

    glm_translate_make (mesh->translation_matrix, (vec3) {0.0f, 0.0f, 0.0f});
    glm_rotate_make (mesh->rotation_matrix, 0.0f, (vec3) {0.0f, 0.0f, 0.0f});
    glm_scale_make (mesh->scaling_matrix, (vec3) {1.0f, 1.0f, 1.0f});

    mesh->animation = in_animation;

    mesh->cache_batch_id = -1;
    mesh->cache_matrix_offset = 0;
    mesh->cache_texcoords_offset = 0;
    
    return (mesh);
}

FGEError FGEMeshDelete (FGEMesh* in_mesh)
{
    assert (in_mesh);

    free (in_mesh->vertices);
    free (in_mesh->indices);
	free (in_mesh->tag);
    free (in_mesh);

    return (FGE_E_SUCCESS);
}

void FGEMeshTranslate (FGEMesh* in_mesh, vec3 in_translation)
{
    assert (in_mesh);

    glm_translate(in_mesh->translation_matrix, in_translation);
}

void FGEMeshRotate (FGEMesh* in_mesh, float theta, vec3 in_axis)
{
    assert (in_mesh);

    glm_rotate(in_mesh->rotation_matrix, theta, in_axis);
}

void FGEMeshScale (FGEMesh* in_mesh, vec3 in_scale)
{
    assert (in_mesh);

    glm_scale(in_mesh->rotation_matrix, in_scale);
}

void FGEMeshSetScale (FGEMesh* in_mesh, vec3 in_scale)
{
    return;
}

void FGEMeshGetMatrix (FGEMesh* in_mesh, mat4 matrix)
{
    assert (in_mesh);

    glm_mat4_mul(in_mesh->translation_matrix, in_mesh->rotation_matrix, matrix);
    glm_mat4_mul(matrix, in_mesh->scaling_matrix, matrix);
}