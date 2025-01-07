#ifndef HEADER_FLING_VECTOR_IMPL
#define HEADER_FLING_VECTOR_IMPL

#include "mesh.h"
#include "camera.h"

#define FLING_VECTOR_TYPE int
#define FLING_VECTOR_PREFIX Int

#include "common/vector_template.h"

#define FLING_VECTOR_TYPE FGEMesh*
#define FLING_VECTOR_PREFIX Mesh

#include "common/vector_template.h"

#define FLING_VECTOR_TYPE FGECamera*
#define FLING_VECTOR_PREFIX Camera

#include "common/vector_template.h"

/* Forward Declare */
typedef struct FGEBatch FGEBatch;

#define FLING_VECTOR_TYPE FGEBatch*
#define FLING_VECTOR_PREFIX Batch

#include "common/vector_template.h"

#endif