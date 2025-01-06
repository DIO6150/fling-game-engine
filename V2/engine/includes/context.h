#ifndef H_MGE_CONTEXT
#define H_MGE_CONTEXT

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "common.h"

/*
    The context is used to create a window.
*/

#ifndef MGE_WINDOW_TITLE
#define MGE_WINDOW_TITLE "MAEL GAME ENGINE"
#endif

#ifndef MGE_WINDOW_SIZE
#define MGE_WINDOW_SIZE 800
#endif

typedef struct MGEContext MGEContext;

struct MGEContext
{
    GLFWwindow* window;
    const char* name;
    int width;
    int height;
};

MGEContext mgeContextInit (char* _in_title, int _in_width, int _in_height);
MGEError mgeContextFree (MGEContext* _in_context);

MGEBool mgeContextShouldClose (MGEContext* _in_context);
MGEBool mgeContextIsFocused (MGEContext* _in_context);

#endif