#ifndef H_MGE_GENERIC
#define H_MGE_GENERIC

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	char* name;
	int hold_generic;
	int is_pointer;
	void* data;
	size_t block_size;
	size_t elem_size;
} GenericData;

GenericData mgeGenericInt(const char* name, int data);
GenericData mgeGenericDouble(const char* name, double data);
GenericData mgeGenericChar(const char* name, int count, ...);
GenericData mgeGenericString(const char* name, char* data);

GenericData mgeGenericPointer(const char* name, void* data);

GenericData mgeGenericList(const char* name, int count, ...);

void* mgeGenericGetData(GenericData data);
int mgeGenericGetCount(GenericData data);

int mgeGenericFree(GenericData data);

#endif