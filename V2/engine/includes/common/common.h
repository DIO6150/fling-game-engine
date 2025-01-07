#ifndef HEADER_FGE_COMMON
#define HEADER_FGE_COMMON

#include "error.h"
#include "bool.h"

typedef enum FGEViewType FGEViewType;

enum FGEViewType
{
	FGE_PERSPECTIVE = 0,
    FGE_ORTHOGRAPHIC = 1
};

#endif