#include <assert.h>

#include "data/event_manager.h"

int mgeEventManagerInit(EventManager** _out_event_bus)
{
	(*_out_event_bus) = malloc(sizeof(EventManager));
	if (!(*_out_event_bus))
    {
        printf("%s(%s) : Event Bus is NULL.\n", __func__, __FILE__);
        return (0);
    }

	(*_out_event_bus)->events = NULL;
	(*_out_event_bus)->events_buffer = NULL;
	(*_out_event_bus)->listeners = NULL;
	
	(*_out_event_bus)->lock = 0;

	return (1);
}

GenericData mgeEventManagerPop(EventManager* _in_event_bus)
{
	if (!_in_event_bus->events)
	{
		return (GenericData) {0, 0, 0, 0, 0};
	}

	EventNode* event = _in_event_bus->events;

	GenericData r_data = event->event_data;

	_in_event_bus->events = _in_event_bus->events->next;

	free(event);

	return r_data;
}

int mgeEventManagerPush(EventManager* _in_event_bus, GenericData _in_event)
{
	if (!(_in_event_bus))
	{
		printf("%s(%s) : Event Bus is NULL.\n", __func__, __FILE__);
        return (0);
	}

	EventNode* event = malloc(sizeof(EventNode));

	if (!(event))
	{
		printf("%s(%s) : Couldn't allocate Event memory.\n", __func__, __FILE__);

		assert(event);

        return (0);
	}

	if (!_in_event_bus->lock)
	{
		event->next = _in_event_bus->events;
		event->event_data = _in_event;

		_in_event_bus->events = event;
	}

	else
	{
		event->next = _in_event_bus->events_buffer;
		event->event_data = _in_event;

		_in_event_bus->events_buffer = event;
	}
	
	return (1);
}

int mgeEventManagerAddListener(EventManager* _in_event_bus, char* event_name, void (*notify_func)(GenericData, EventManager*))
{
	if (!(_in_event_bus))
	{
		printf("%s(%s) : Event Bus is NULL.\n", __func__, __FILE__);
        return (0);
	}

	ListenerNode* next_listener = _in_event_bus->listeners;

	_in_event_bus->listeners = malloc(sizeof(ListenerNode));

	if (!(_in_event_bus->listeners))
	{
		printf("%s(%s) : Couldn't allocate listener memory.\n", __func__, __FILE__);
		_in_event_bus->listeners = next_listener;
        return (0);
	}

	_in_event_bus->listeners->next = next_listener;
	_in_event_bus->listeners->event_name = event_name;
	_in_event_bus->listeners->notify = notify_func;

	return (1);
}

// TODO : only for debug, delete later

static int __attribute__((unused)) size_of_events(EventNode* node)
{
	if (node == NULL) return (0);

	return (1 + size_of_events(node->next));
}

int mgeEventManagerHandleEvents(EventManager* _in_event_bus)
{
	if (!(_in_event_bus))
	{
		printf("%s(%s) : Event Bus is NULL.\n", __func__, __FILE__);
        return (0);
	}

	_in_event_bus->lock = 1;

	while (_in_event_bus->events)
	{
		GenericData event = mgeEventManagerPop(_in_event_bus);

		ListenerNode* node = _in_event_bus->listeners;

		while (node)
		{
			if (!strcmp(event.name, node->event_name) && node->notify) node->notify(event, _in_event_bus);

			node = node->next;
		}

		mgeGenericFree(event);
	}

	_in_event_bus->lock = 0;

	_in_event_bus->events =_in_event_bus->events_buffer;
	_in_event_bus->events_buffer = NULL;

	return (1);
}

static void free_events(EventNode* node)
{
	if (!node) return;

	free_events(node->next);

	mgeGenericFree(node->event_data);
	node = NULL;
}

static void free_listeners(ListenerNode* node)
{
	if (!node) return;

	free_listeners(node->next);

	free(node);
	node = NULL;
}


int mgeEventManagerCleanEvents(EventManager* _in_event_bus)
{
	if (!(_in_event_bus))
	{
		printf("%s(%s) : Event Bus is NULL.\n", __func__, __FILE__);
        return (0);
	}

	free_events(_in_event_bus->events);

	return (1);
}

int mgeEventManagerCleanListener(EventManager* _in_event_bus)
{
	if (!(_in_event_bus))
	{
		printf("%s(%s) : Event Bus is NULL.\n", __func__, __FILE__);
        return (0);
	}

	free_listeners(_in_event_bus->listeners);

	return (1);
}

int mgeEventManagerFree(EventManager* _in_event_bus)
{
	if (!_in_event_bus)
	{
		printf("%s(%s) : Event Bus is NULL.\n", __func__, __FILE__);
        return (0);
	}

	mgeEventManagerCleanEvents(_in_event_bus);
	mgeEventManagerCleanListener(_in_event_bus);

	free(_in_event_bus);

	return (1);
}