#ifndef H_MGE_event_manager
#define H_MGE_event_manager

#include "common.h"

// Forward Declaration
typedef struct MGEEngine MGEEngine;

typedef struct MGEEventNode MGEEventNode;
typedef struct MGEListenerNode MGEListenerNode;

typedef struct MGEEventManager MGEEventManager;

typedef struct MGEEventData MGEEventData;

struct MGEEventData
{
	char event_name[256]; // optional
	int event_id;
	void* data;
	int count;
	size_t block_size;
	size_t element_size;
};

struct MGEEventNode
{
	MGEEventData data;
	MGEEventNode* next;
};

struct ListenerNode
{
	int event_id;
	void (*notify)(MGEEventData, MGEEngine*);

	struct ListenerNode* next;
};

struct MGEEventManager
{
	MGEEventNode* events_buffer_front;
	MGEEventNode* events_buffer_back;
	MGEListenerNode* listeners;

	MGEEngine* engine;

	int lock;
};

MGEEventManager mgeEventManagerInit(MGEEngine* _in_engine);
MGEError mgeEventManagerFree(MGEEventManager* _in_event_manager);

MGEEventData mgeEventManagerPop(MGEEventManager* _in_event_manager);
void mgeEventManagerPush(MGEEventManager* _in_event_manager, MGEEventData _in_event);

void mgeEventManagerNotify(MGEEventManager* _in_event_manger, MGEEventData _in_event);

void mgeEventManagerAddListener(MGEEventManager* _in_event_manager, int event_id, void (*notify_func)(MGEEventData, MGEEngine));

MGEError mgeEventManagerHandleEvents(MGEEventManager* _in_event_manager);

MGEError mgeEventManagerCleanEvents(MGEEventManager* _in_event_manager);
MGEError mgeEventManagerCleanListener(MGEEventManager* _in_event_manager);


#endif