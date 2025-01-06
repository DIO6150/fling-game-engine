#ifndef H_MGE_COMMON
#define H_MGE_COMMON

#include <stdlib.h>

typedef enum MGEError MGEError;
typedef enum MGEBool MGEBool;
typedef enum MGEViewType MGEViewType;

typedef struct MGEArray MGEArray;
typedef struct MGETree MGETree;
typedef struct MGETreeNode MGETreeNode;

enum MGEError
{
    MGE_E_NONE = 0,
    MGE_E_SUCCESS = 1,
    MGE_E_INIT_FAIL = 2,
    MGE_E_OUT_OF_MEMORY = 3,
    MGE_E_WRONG_SIZE = 4,
    MGE_E_SHADER_COMPILE = 5
};

enum MGEBool
{
    MGE_FALSE = 0,
    MGE_TRUE = 1
};

enum MGEViewType
{
	MGE_PERSPECTIVE = 0,
    MGE_ORTHOGRAPHIC = 1
};

struct MGEArray
{
    void* data;
    size_t element_size;
    
    int count;
    int max_count;

    void* start; // is an offset into data
};

struct MGETreeNode
{
    void* data;
    size_t element_size;
    size_t block_size;

    int tag;
    unsigned int height;
};

struct MGETree
{
    MGETreeNode* first_node;
};

// 100 Mo
#define MGE_ARRAY_SIZE_LIMIT 100000000

MGEError mgeArrayInit (MGEArray* _out_array, size_t _in_element_size);
MGEError mgeArrayFree (MGEArray* _in_array);

MGEError mgeArrayPushBack (MGEArray* _in_array, void* _in_data, size_t _in_size);

void* mgeArrayPopFront (MGEArray* _in_array);
MGEError mgeArrayPopIn (MGEArray* _in_array, void* _out_data);

MGETree* mgeTreeInit ();
MGEError mgeTreeFree (MGETree* _in_tree);

MGEError mgeTreeGet(MGETree* _in_tree, int _in_tag, size_t size, void* _out_data);
int mgeTreeAdd(MGETree* _in_tree, size_t size, void* _in_data);

#endif