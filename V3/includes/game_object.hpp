#pragma once

#include <iostream>
#include <string>

#include "engine.hpp"
#include "igame_object.hpp"

namespace FGE
{
    template <class T>
    class GameObject : public IGameObject
    {
    public:
        GameObject (std::string id, T&& object)
        {
            Engine* e = Engine::GetInstance();

            m_id = id;
            m_object = T(object);

            e->RegisterGameObject<T> (id, this);
        }

        ~GameObject ()
        {

        }

        T m_object;

    private:
        std::string m_id;
    };
}