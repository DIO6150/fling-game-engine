#include "string.h"

#include "includes/common.h"

#include "stdlib.h"

static MGEError extend_array (void* array, size_t element_size, int* count, int* max_count)
{
    (*max_count) *= 2;
    void* realloc_data = realloc (array, element_size * (*max_count));

    if (!realloc_data) return (MGE_E_INIT_FAIL);

    array = realloc_data;

    return (MGE_E_SUCCESS);
}

MGEArray* mgeArrayInit (size_t _in_element_size)
{
    MGEArray* array = malloc (sizeof (MGEArray));

    assert (array);

    array->count = 0;
    array->max_count = 16;
    array->element_size = _in_element_size;
    array->data = malloc (_in_element_size * 16);

    array->start = array->data;

    assert (array->data);

    return (MGE_E_SUCCESS);
}

MGEError mgeArrayFree (MGEArray* _in_array)
{
    assert (_in_array);

    free (_in_array->data);

    return (MGE_E_SUCCESS);
}

MGEError mgeArrayPushBack (MGEArray* _in_array, void* _in_data, size_t _in_size)
{
    if (_in_size != _in_array->element_size) return (MGE_E_WRONG_SIZE);

    if ( (_in_array->count + 1) * _in_array->element_size >= MGE_ARRAY_SIZE_LIMIT)
    {
        assert (NULL);

        return (MGE_E_OUT_OF_MEMORY);
    }

    // TODO : it might get over 100 Mo in size if previous size is like 95 Mo with an int

    if (_in_array->count + 1 >= _in_array->max_count)
    {
        _in_array->max_count *= 2;
        void* realloc_data = realloc (_in_array->data, _in_array->element_size * _in_array->max_count);

        // redundant

        assert (realloc_data);

        if (!realloc_data)
        {
            return (MGE_E_INIT_FAIL);
        }

        _in_array->data = realloc_data;
    }

    memcpy (_in_array->data + _in_array->element_size * _in_array->count, _in_data, _in_size);

    return (MGE_E_SUCCESS);
}

void* mgeArrayPopFront (MGEArray* _in_array)
{
    void* r_data = _in_array->start;

    _in_array->start += _in_array->element_size;

    return (r_data);
}

MGEError mgeArrayPopIn (MGEArray* _in_array, void* _out_data);