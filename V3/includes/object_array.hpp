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
        /**
         * @brief Construct a new ObjectArray
         * 
         */
        ObjectArray ()
        {
            m_objects = std::unordered_map<std::string, T>();
        }

        /**
         * @brief Destroy the ObjectArray
         * 
         */
        ~ObjectArray ()
        {

        }

        /**
         * @brief Insert at the key key an element of type T
         * 
         * @param key The key referencing the object.
         * @param object The object to insert.
         */
        void Insert (std::string key, T& object)
        {
            m_objects.insert_or_assign (key, std::move(object));
        }

        /**
         * @brief Returns a pointer to an object of type T matching the corresponding key.
         * 
         * @param key The key to find
         * @return T*
         */
        T* Find (std::string key)
        {
            auto it = m_objects.find (key);
            return (it != m_objects.end () ? &it->second : nullptr);
        }

        /**
         * @brief Remove the element matching the key
         * 
         * @param key Key of the object to remove
         */
        void Remove (std::string key)
        {
            m_objects.erase (key);
        }

        /**
         * @brief Clear all the elements
         * 
         */
        void Clear ()
        {
            m_objects.clear ();
        }


        // Neither of those works

        auto Begin ()
        {
            return (m_objects.begin ());
        }

        auto End ()
        {
            return (m_objects.end ());
        }
    };
}