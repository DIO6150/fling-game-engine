#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data/generic.h"

GenericData mgeGenericInt(const char* name, int data)
{
	// Create data block
	GenericData g_data;
	g_data.name = (char*) name;
	g_data.hold_generic = 0;
	g_data.is_pointer = 0;
	g_data.elem_size = sizeof(int);
	g_data.block_size = g_data.elem_size;

	g_data.data = (void*) malloc(g_data.block_size);

	// Write data to memory

	int d = data;

	memcpy(g_data.data, &d, g_data.elem_size);
	
	return g_data;
}

GenericData mgeGenericDouble(const char* name, double data)
{
	// Create data block
	GenericData g_data;
	g_data.name = (char*) name;
	g_data.hold_generic = 0;
	g_data.is_pointer = 0;
	g_data.elem_size = sizeof(double);
	g_data.block_size = g_data.elem_size;

	g_data.data = (void*) malloc(g_data.block_size);

	// Write data to memory

	double d = data;

	memcpy(g_data.data, &d, g_data.elem_size);
	
	return g_data;
}

GenericData mgeGenericChar(const char* name, int count, ...)
{
	return (GenericData) {0, 0, 0, 0, 0};
}

GenericData mgeGenericString(const char* name, char* data)
{
	/// Create data block
	GenericData g_data;
	g_data.name = (char*) name;
	g_data.hold_generic = 0;
	g_data.is_pointer = 0;
	g_data.elem_size = sizeof(char);
	g_data.block_size = g_data.elem_size * (strlen(data) + 1);

	g_data.data = (void*) malloc(g_data.block_size);

	// Write data to memory

	memcpy(g_data.data, data, g_data.block_size);
	
	return g_data;
}

GenericData mgeGenericPointer(const char* name, void* data)
{
	// Create data block
	GenericData g_data;
	g_data.name = (char*) name;
	g_data.hold_generic = 0;
	g_data.is_pointer = 1;
	g_data.elem_size = sizeof(void*);
	g_data.block_size = g_data.elem_size;

	// Write data to memory
	
	g_data.data = data;
	
	return g_data;
}

GenericData mgeGenericList(const char* name, int count, ...)
{
	// Create data block
	GenericData g_data;
	g_data.name = (char*) name;
	g_data.hold_generic = 1;
	g_data.is_pointer = 0;
	g_data.elem_size = sizeof(GenericData);
	g_data.block_size = g_data.elem_size * count;

	g_data.data = (void*) malloc(g_data.block_size);

	// Write data to memory
	va_list args;
	va_start(args, count);

	void* write_ptr = g_data.data;

	for (int i = 0; i < count; i++)
	{
		GenericData d = va_arg(args, GenericData);
		memcpy(write_ptr, &d, g_data.elem_size);

		write_ptr += g_data.elem_size;
	}

	va_end(args);

	return g_data;
}

void* mgeGenericGetData(GenericData data)
{
	return data.data;
}

int mgeGenericGetCount(GenericData data)
{
	return data.block_size / data.elem_size;
}

int mgeGenericFree(GenericData data)
{
	if (!data.data) return (0);
	if (data.is_pointer) return (0);

	if (data.hold_generic)
	{
		for (int i = 0; i < mgeGenericGetCount(data); i++)
		{
			GenericData sub_data = ((GenericData*) data.data)[i];
			mgeGenericFree( sub_data );
		}
	}

	free(data.data);

	return (1);
}