#include "batch.h"

typedef struct
{
    unsigned int count;
    unsigned int instance_count;
    unsigned int first_index;
    int base_vertex;
    unsigned int base_instance;
} DrawCommand;
