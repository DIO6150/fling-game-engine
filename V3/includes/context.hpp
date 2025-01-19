#pragma once

#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace FGE
{
    class Context
    {
    public:
        Context (int width, int height, std::string title);
        Context (int size, std::string title) : Context (size, size, title) {}
        Context (int width, int height) : Context (width, height, "Test") {}
        Context (int size) : Context (size, size, "Test") {}
        Context () : Context (800, 800, "Test") {}

        GLFWwindow* GetWindow ();

    private:
        GLFWwindow* m_window;
    };
}