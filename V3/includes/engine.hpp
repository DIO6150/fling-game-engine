#pragma once

#include <memory>
#include <mutex>
#include <vector>
#include <unordered_map>

#include "batch.hpp"
#include "context.hpp"
#include "igame_object.hpp"

namespace FGE
{
    class Engine
    {
        static Engine* m_instance;
        static std::mutex m_mtx;
        Engine ()
        {
            m_context = std::make_unique<Context> ();
        }

        std::vector<Batch> m_batches;
        std::unique_ptr<Context> m_context;

        // <typename, <key, game_object*>>
        std::unordered_map<std::string, std::unordered_map<std::string, IGameObject*>> m_game_objects;
        

    public:
        Engine(const Engine& obj) = delete;
        static Engine* GetInstance ()
        {
            if (Engine::m_instance == nullptr)
            {
                std::lock_guard<std::mutex> lock (Engine::m_mtx);

                if (Engine::m_instance == nullptr)
                {
                    Engine::m_instance = new Engine ();
                }
            }

            return (Engine::m_instance);
        }
    
        ~Engine();

        bool ShouldWindowClose () { return (glfwWindowShouldClose (m_context->GetWindow())); }

        template <class T>
        void RegisterGameObject (std::string key, IGameObject* game_object)
        {
            std::string type_name = std::string (typeid (T).name ());

            std::unordered_map<std::string, IGameObject*> &inner = m_game_objects [type_name];

            if (inner.find (key) != inner.end ()) std::cout << "Could not register GameObject " << key << " (key already exists)" << std::endl;

            inner [key] = game_object;
        }

        template <class T>
        void UnregisterGameObject (std::string key)
        {
            std::string type_name = std::string (typeid (T).name ());

            std::unordered_map<std::string, IGameObject*> &inner = m_game_objects [type_name];

            if (inner.find (key) == inner.end ()) std::cout << "Could not remove GameObject " << key << " (key does not exist)" << std::endl;

            inner.erase (key);
        }

        void PrintDebug ();

        void Render ();

        void LoadBatches ();
    };
}