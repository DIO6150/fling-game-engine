#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "context.h"

static void message_callback (__attribute__((unused))GLenum source, GLenum type, GLuint id, GLenum severity, __attribute__((unused))GLsizei length, const GLchar    * message, __attribute__((unused))const void* userParam)
{
    printf ("Severity : %d, Type : %d, ID : %d, Message : %s\n", severity, type, id, message);
}

void framebuffer_size_callback (__attribute__((unused))GLFWwindow* window, int width, int height)
{
    glViewport (0, 0, width, height);
}

static void glfw_error_callback (int code, const char* message)
{
    printf ("[GLFW ERROR] : {%d} : %s\n", code, message);
}

FGEContext* FGEContextNew (char* in_title, int in_width, int in_height)
{
    FGEContext* context = malloc (sizeof (FGEContext));

    if (context == NULL)
    {
        printf ("%s(%s) : Couldn't allocate memory.\n", __func__, __FILE__);
        return (0);
    }

    glfwSetErrorCallback (glfw_error_callback);

    #ifdef __linux__
    glfwInitHint (GLFW_PLATFORM, GLFW_PLATFORM_X11);
    #endif

    if (!glfwInit ())
    {
        free (context);
        printf ("%s(%s) : GLFW failed to initialize.\n", __func__, __FILE__);
        return (0);
    }
  
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint (GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    context->window = glfwCreateWindow (in_width, in_height, in_title, NULL, NULL);

    context->width = in_width;
    context->height = in_height;

    if (!context->window)
    {
        free (context);
        glfwTerminate ();
        printf ("%s(%s) : Failed to create window.\n", __func__, __FILE__);
        return (0); 
    }

    // glfwSetWindowAttrib(context->window, GLFW_RESIZABLE, GLFW_FALSE);

    glfwMakeContextCurrent (context->window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwDestroyWindow (context->window);
        free (context);
        glfwTerminate ();
        
        printf ("%s(%s) : Failed to initialize glad.\n", __func__, __FILE__);
        return(0);
    }
    printf ("GL %d.%d\n", GLVersion.major, GLVersion.minor);
    
    glViewport (0, 0, in_width, in_height);
    
    glfwSetFramebufferSizeCallback (context->window, framebuffer_size_callback);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LESS);
    //glCullFace (GL_FRONT);
    //glFrontFace (GL_CW);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable (GL_DEBUG_OUTPUT);
    glDebugMessageCallback (message_callback, 0);
    glDebugMessageControl (GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0, NULL, GL_TRUE);

    return context;
}

FGEError FGEContextDelete (FGEContext* in_context)
{
    if (!in_context)
    {
        printf ("%s(%s) : Context is NULL.\n", __func__, __FILE__);
        return (FGE_E_INIT_FAIL);
    }

    if (!in_context->window)
    {
        printf ("%s(%s) : Context window is NULL.\n", __func__, __FILE__);
        return (FGE_E_INIT_FAIL);
    }

    glfwDestroyWindow (in_context->window);

    free (in_context);

    glfwTerminate ();


    return (FGE_E_SUCCESS);
}

FGEBool FGEContextShouldClose (FGEContext* in_context)
{
    if (!in_context)
    {
        printf ("%s(%s) : Context is NULL.\n", __func__, __FILE__);
        
        assert (0);
    }

    if (!in_context->window)
    {
        printf ("%s(%s) : Context window is NULL.\n", __func__, __FILE__);

        assert (0);
    }

    return (glfwWindowShouldClose (in_context->window) ? FGE_TRUE : FGE_FALSE);
}

FGEBool FGEContextIsFocused (FGEContext* in_context)
{
    return (FGE_TRUE);
}