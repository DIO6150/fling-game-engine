#ifndef H_MGE_EVENT_BUS
#define H_MGE_EVENT_BUS

#include "data/generic.h"

typedef struct EventManager EventManager;

typedef struct EventNode
{
	GenericData event_data;

	struct EventNode* next;
} EventNode;

typedef struct ListenerNode
{
	char* event_name;
	void (*notify)(GenericData, EventManager*);

	struct ListenerNode* next;
} ListenerNode;

typedef struct EventManager
{
	EventNode* events;
	EventNode* events_buffer;
	ListenerNode* listeners;

	int lock;
} EventManager;

int mgeEventManagerInit(EventManager** _out_event_bus);

GenericData mgeEventManagerPop(EventManager* _in_event_bus);
int mgeEventManagerPush(EventManager* _in_event_bus, GenericData _in_event);

int mgeEventManagerAddListener(EventManager* _in_event_bus, char* event_name, void (*notify_func)(GenericData, EventManager*));

int mgeEventManagerHandleEvents(EventManager* _in_event_bus);

int mgeEventManagerCleanEvents(EventManager* _in_event_bus);
int mgeEventManagerCleanListener(EventManager* _in_event_bus);

int mgeEventManagerFree(EventManager* _in_event_bus);

#endif