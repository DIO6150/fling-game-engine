#pragma once

#include <string>
#include <unordered_map>

namespace FGE
{
    class IObjectArray
    {
    public:
        virtual ~IObjectArray () = default;
    };

    template <class T>
    class ObjectArray: public IObjectArray
    {
        std::unordered_map<std::string, T> m_objects;

    public:
        ObjectArray ()
        {
            m_objects = std::unordered_map<std::string, T>();
        }

        ~ObjectArray ()
        {

        }

        void Insert (std::string key, T& object)
        {
            printf ("test 1\n");
            m_objects.insert_or_assign (key, std::move(object));
            printf ("test 2\n");
        }

        void Remove (std::string key)
        {
            m_objects.erase (key);
        }

        void Clear ()
        {
            m_objects.clear ();
        }

        auto Begin ()
        {
            return (m_objects.begin ());
        }

        auto End ()
        {
            return (m_objects.end ());
        }


        T* Find (std::string key)
        {
            auto it = m_objects.find (key);
            return (it != m_objects.end () ? &it->second : nullptr);
        }
    };
}