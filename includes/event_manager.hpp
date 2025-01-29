#pragma once

#include <string>
#include <unordered_map>
#include <queue>

namespace FGE
{
    class IEvent
    {
        ~IEvent() = default;
    };

    template <class T>
    class Event : IEvent
    {
        T m_object;
    public:
        Event (T& object)
        {
            m_object = T(object);
        }
    };

    class EventManager
    {
        std::unordered_map<std::string, int(*)(IEvent*)> m_observers;

        std::unordered_map<std::string, std::queue<IEvent*>> m_events_front;
        std::unordered_map<std::string, std::queue<IEvent*>> m_events_back;

        bool lock;

    public:
        EventManager ();
        ~EventManager ();

        template <class T>
        void AddEvent (T event)
        {
            Event<T>* e = new Event<T> (event);
            IEvent* display_event = dynamic_cast<IEvent*> (e);

            std::string type_name = typeid (T).name ();

            std::unordered_map<std::string, std::queue<IEvent*>>* event_buffer;

            if (lock) event_buffer = &m_events_back;
            else event_buffer = &m_events_front;

            if (event_buffer.find (type_id) == m_events_buffer.end())
            {
                event_buffer [typeid] = std::queue<IEvent*> ();
            }

            event_buffer [typeid].push (display_event);
        }

        template <class T>
        void RegisterObserver (int(*listener)(T*));

        void Handle ();
    };
}