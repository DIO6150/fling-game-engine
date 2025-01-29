#pragma once

#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace FGE
{
    class Context
    {
    public:
        /**
         * @brief Construct a new Context object
         * 
         * @param width Width of the window to be created.
         * @param height Height of the window to be created.
         * @param title Title of the window to be created.
         */
        Context (int width, int height, std::string title);

        /**
         * @brief Construct a new Context object
         * 
         * @param size Both width and height of the window to be created.
         * @param title Title of the window to be created
         */
        Context (int size, std::string title) : Context (size, size, title) {}

        /**
         * @brief Construct a new Context object. The window will be named "Fling Game Engine"
         * 
         * @param width Width of the window to be created.
         * @param height Height of the window to be created.
         */
        Context (int width, int height) : Context (width, height, "Fling Game Engine") {}

        /**
         * @brief Construct a new Context object. The window will be named "Fling Game Engine"
         * 
         * @param size Both width and height of the window to be created.
         */
        Context (int size) : Context (size, size, "Fling Game Engine") {}

        /**
         * @brief Construct a new Context object. The window will be named "Fling Game Engine" and will both have a width and a height of 800px.
         * 
         */
        Context () : Context (800, 800, "Fling Game Engine") {}

        /**
         * @brief Returns the GLFW window object.
         * 
         * @return GLFWwindow* 
         */
        GLFWwindow* GetWindow ();

    private:
        GLFWwindow* m_window;
    };
}