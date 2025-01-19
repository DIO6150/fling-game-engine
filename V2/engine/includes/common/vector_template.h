
#ifndef HEADER_FGE_VECTOR
#define HEADER_FGE_VECTOR
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include "common/error.h"
    #include "common/bool.h"

    #define XCONCAT(A, B) A##B
    #define CONCAT(A, B) XCONCAT(A, B)

    #define XSTR(A) #A
    #define STR(A) XSTR(A)

    #ifndef FGE_VECTOR_SIZE_LIMIT
        /* 100 Mo = 100.000.000 o*/
        #define FGE_VECTOR_SIZE_LIMIT 100000000
    #endif
#endif

/* DEBUG ZONE */


/* TEMPLATED NAMES */

#ifndef FLING_VECTOR_TYPE
    #error "Key type must defined."
#endif

#ifndef FLING_VECTOR_PREFIX
    #error "Must give a proper prefix."
#endif


#define FLING_VECTOR_SUFFIX CONCAT(FLING_VECTOR_PREFIX, Vector)
#define FLING_VECTOR_NAME CONCAT(FGE, FLING_VECTOR_SUFFIX)
#undef  FLING_VECTOR_NAME_SUFFIX

#define FLING_VECTOR_IMPL_NAME(A) (CONCAT(FLING_VECTOR_NAME, A))

#define FLING_VECTOR_NEW FLING_VECTOR_IMPL_NAME(New)
#define FLING_VECTOR_DELETE FLING_VECTOR_IMPL_NAME(Delete)
#define FLING_VECTOR_PUSH FLING_VECTOR_IMPL_NAME(Push)
#define FLING_VECTOR_POP FLING_VECTOR_IMPL_NAME(Pop)
#define FLING_VECTOR_GET FLING_VECTOR_IMPL_NAME(Get)
#define FLING_VECTOR_CONTAINS FLING_VECTOR_IMPL_NAME(Contains)

/* TEMPLATED DATA STRUCTURES */

typedef struct FLING_VECTOR_NAME FLING_VECTOR_NAME;

struct FLING_VECTOR_NAME
{
    FLING_VECTOR_TYPE* m_data;
    size_t m_size;
    size_t m_max_size;

    int (*m_compare) (FLING_VECTOR_TYPE, FLING_VECTOR_TYPE);
};

/* TEMPLATED FUNCTIONS */

static inline FLING_VECTOR_NAME FLING_VECTOR_NEW (int (in_compare) (FLING_VECTOR_TYPE, FLING_VECTOR_TYPE))
{
    FLING_VECTOR_NAME vector;

    vector.m_size = 0;
    vector.m_max_size = 8;
    vector.m_data = (FLING_VECTOR_TYPE*) malloc (sizeof (FLING_VECTOR_TYPE) * vector.m_max_size);
    vector.m_compare = in_compare;

    assert (vector.m_data);

    return (vector);
}

static inline FGEError FLING_VECTOR_DELETE (FLING_VECTOR_NAME* in_vector)
{
    assert (in_vector);

    free (in_vector->m_data);

    return (FGE_E_SUCCESS);
}

static inline FGEError FLING_VECTOR_PUSH (FLING_VECTOR_NAME* in_vector, FLING_VECTOR_TYPE in_data)
{
    if ((in_vector->m_size + 1) * sizeof (FLING_VECTOR_TYPE) > FGE_VECTOR_SIZE_LIMIT) 
        return (FGE_E_CONTAINER_LIMIT);

    if (in_vector->m_size + 1 > in_vector->m_max_size)
    {
        in_vector->m_max_size *= 2;
        in_vector->m_data = (FLING_VECTOR_TYPE*) realloc (in_vector->m_data, in_vector->m_max_size);
    }

    in_vector->m_data [in_vector->m_size++] = in_data;

    return (FGE_E_SUCCESS);
}

static inline FLING_VECTOR_TYPE FLING_VECTOR_POP (FLING_VECTOR_NAME* in_vector, void* out_data)
{
    if (in_vector->m_size <= 0)
    {
        printf("%s(%s) : FGE_E_CONTAINER_EMPTY\n", __func__, __FILE__);
        
        assert (0);
    }

    return (in_vector->m_data [--in_vector->m_size]);
}

static inline FLING_VECTOR_TYPE FLING_VECTOR_GET (FLING_VECTOR_NAME* in_vector, int in_index)
{
    if (in_index >= in_vector->m_size || in_index < 0)
    {
        printf("%s(%s) : FGE_E_CONTAINER_OUTSIDE_RANGE\n", __func__, __FILE__);
        
        assert (0);
    }

    return (in_vector->m_data [in_vector->m_size - 1]);
}

static inline FGEBool FLING_VECTOR_CONTAINS (FLING_VECTOR_NAME* in_vector, FLING_VECTOR_TYPE in_value)
{
    for (int i = 0; i < in_vector->m_size; ++i)
    {
        if (!in_vector->m_compare (in_vector->m_data [i], in_value))
        {
            return (FGE_TRUE);
        }
    }

    return (FGE_FALSE);
}

#undef FLING_VECTOR_TYPE
#undef FLING_VECTOR_PREFIX

#undef FLING_VECTOR_NAME
#undef FLING_VECTOR_IMPL_NAME

#undef FLING_VECTOR_NEW
#undef FLING_VECTOR_DELETE
#undef FLING_VECTOR_PUSH
#undef FLING_VECTOR_POP
#undef FLING_VECTOR_GET
#undef FLING_VECTOR_CONTAINS