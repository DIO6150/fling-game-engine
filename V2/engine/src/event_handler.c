#include <stdlib.h>

#include "event_handler.h"

FGEEventHandler* FGEEventHandlerNew (FGEEngine* in_engine)
{
    return (NULL);
}

FGEError FGEEventHandlerDelete (FGEEventHandler* in_event_handler)
{
    return (0);
}

FGEEventData FGEEventHandlerPop (FGEEventHandler* in_event_handler)
{
    return ((FGEEventData){0});
}

void FGEEventHandlerPush (FGEEventHandler* in_event_handler, FGEEventData in_event)
{
    return;
}

void FGEEventHandlerNotify (FGEEventHandler* in_event_manger, FGEEventData in_event)
{
    return;
}

void FGEEventHandlerAddListener (FGEEventHandler* in_event_handler, int event_id, void  (*notify_func) (FGEEventData, FGEEngine))
{
    return;
}

FGEError FGEEventHandlerHandleEvents (FGEEventHandler* in_event_handler)
{
    return (0);
}

FGEError FGEEventHandlerClearEvents (FGEEventHandler* in_event_handler)
{
    return (0);
}

FGEError FGEEventHandlerClearListener (FGEEventHandler* in_event_handler)
{
    return (0);
}