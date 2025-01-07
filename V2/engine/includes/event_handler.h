#ifndef HEADER_FGE_EVENT_HANDLER
#define HEADER_FGE_EVENT_HANDLER

#include "common/common.h"

// Forward Declaration
typedef struct FGEEngine FGEEngine;

typedef struct FGEEventNode FGEEventNode;
typedef struct FGEListenerNode FGEListenerNode;

typedef struct FGEEventHandler FGEEventHandler;

typedef struct FGEEventData FGEEventData;

struct FGEEventData
{
	char event_name[256]; // optional
	int event_id;
	void* data;
	int count;
	size_t block_size;
	size_t element_size;
};

struct FGEEventNode
{
	FGEEventData data;
	FGEEventNode* next;
};

struct ListenerNode
{
	int event_id;
	void (*notify) (FGEEventData, FGEEngine*);

	struct ListenerNode* next;
};

struct FGEEventHandler
{
	FGEEventNode* events_buffer_front;
	FGEEventNode* events_buffer_back;
	FGEListenerNode* listeners;

	FGEEngine* engine;

	int lock;
};

FGEEventHandler* FGEEventHandlerNew (FGEEngine* in_engine);
FGEError FGEEventHandlerDelete (FGEEventHandler* in_event_handler);

FGEEventData FGEEventHandlerPop (FGEEventHandler* in_event_handler);
void FGEEventHandlerPush (FGEEventHandler* in_event_handler, FGEEventData in_event);

void FGEEventHandlerNotify (FGEEventHandler* in_event_manger, FGEEventData in_event);

void FGEEventHandlerAddListener (FGEEventHandler* in_event_handler, int event_id, void  (*notify_func) (FGEEventData, FGEEngine));

FGEError FGEEventHandlerHandleEvents (FGEEventHandler* in_event_handler);

FGEError FGEEventHandlerClearEvents (FGEEventHandler* in_event_handler);
FGEError FGEEventHandlerClearListener (FGEEventHandler* in_event_handler);


#endif