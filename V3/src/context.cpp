
#include <iostream>

#include "context.hpp"

static void message_callback (__attribute__((unused))GLenum source, GLenum type, GLuint id, GLenum severity, __attribute__((unused))GLsizei length, const GLchar    * message, __attribute__((unused))const void* userParam)
{
    printf ("Severity : %d, Type : %d, ID : %d, Message : %s\n", severity, type, id, message);
}

void framebuffer_size_callback (__attribute__((unused))GLFWwindow* m_window, int width, int height)
{
    glViewport (0, 0, width, height);
}

static void glfw_error_callback (int code, const char* message)
{
    printf ("[GLFW ERROR] : {%d} : %s\n", code, message);
}

FGE::Context::Context (int width, int height, std::string title)
{
    glfwSetErrorCallback (glfw_error_callback);

    #ifdef __linux__
    glfwInitHint (GLFW_PLATFORM, GLFW_PLATFORM_X11);
    #endif

    if (!glfwInit ())
    {
        std::cout << __FILE__ << ": (" << __func__ << "): " << "GLFW failed to initialize glfw.";
        return;
    }
  
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint (GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    m_window = glfwCreateWindow (width, height, title.c_str(), NULL, NULL);

    if (!m_window)
    {
        glfwTerminate ();
        std::cout << __FILE__ << ": (" << __func__ << "): " << "GLFW failed to create m_window.";
        return; 
    }

    // glfwSetWindowAttrib(context->m_window, GLFW_RESIZABLE, GLFW_FALSE);

    glfwMakeContextCurrent (m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwDestroyWindow (m_window);
        glfwTerminate ();
        
        std::cout << __FILE__ << ": (" << __func__ << "): " << "GLFW failed to initialize glad.";
        return;
    }
    
    glViewport (0, 0, width, height);
    
    glfwSetFramebufferSizeCallback (m_window, framebuffer_size_callback);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LESS);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable (GL_DEBUG_OUTPUT);
    glDebugMessageCallback (message_callback, 0);
    glDebugMessageControl (GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0, NULL, GL_TRUE);
}

GLFWwindow* FGE::Context::GetWindow ()
{
    return (m_window);
}