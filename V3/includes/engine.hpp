#pragma once

#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "batch.hpp"
#include "context.hpp"
#include "object_array.hpp"
#include "scene.hpp"

namespace FGE
{
    class Engine
    {
        static Engine* m_instance;
        static std::mutex m_mtx;
        Engine ()
        {
            m_context = std::make_unique<Context> ();

            m_scenes.push_back (std::make_shared<Scene> ());
            active_scene = m_scenes[0];
        }

        std::vector<Batch> m_batches;
        std::unique_ptr<Context> m_context;

        // <typename, <key, game_object>>
        std::unordered_map<std::string, IObjectArray*> m_game_objects;
        
        std::vector<std::shared_ptr<Scene>> m_scenes;
        std::shared_ptr<Scene> active_scene;

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

        static bool ShouldWindowClose ();

        static std::shared_ptr<Scene> GetActiveScene ();

        template <class T>
        static void RegisterGameObject (std::string key, T& game_object)
        {
            Engine* engine = Engine::GetInstance();

            std::string type_name = std::string (typeid (T).name ());

            if (engine->m_game_objects.find (type_name) == engine->m_game_objects.end ())
            {
                engine->m_game_objects [type_name] = dynamic_cast<IObjectArray*> (new ObjectArray<T>);
            }

            ObjectArray<T>* inner = dynamic_cast<ObjectArray<T>*> (engine->m_game_objects [type_name]);
            inner->Insert (key, game_object);
        }

        template <class T>
        static void UnregisterGameObject (std::string key)
        {
            Engine* engine = Engine::GetInstance();

            std::string type_name = std::string (typeid (T).name ());

            if (engine->m_game_objects.find (type_name) == engine->m_game_objects.end ())
            {
                return;
            }

            ObjectArray<T>* inner = dynamic_cast<ObjectArray<T>*> (engine->m_game_objects [type_name]);
            inner->Remove (key);
        }

        template <class T>
        static T* FindGameObject (std::string key)
        {
            Engine* engine = Engine::GetInstance();

            std::string type_name = std::string (typeid (T).name ());

            if (engine->m_game_objects.find (type_name) == engine->m_game_objects.end ())
            {
                return (nullptr);
            }

            ObjectArray<T>* inner = dynamic_cast<ObjectArray<T>*> (engine->m_game_objects [type_name]);
            return (inner->Find (key));
        }

        static void UploadMesh (std::string key, glm::vec3 translation = {0.0, 0.0, 0.0}, glm::vec3 rotation = {0.0, 0.0, 0.0}, glm::vec3 scaling = {1.0, 1.0, 1.0});

        static void LoadBatches (std::string shader_key);
        
        static void Render ();
    };
}