#ifndef HEADER_FGE_CONTEXT
#define HEADER_FGE_CONTEXT

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "common/common.h"

/*
    The context is used to create a window.
*/

typedef struct FGEContext FGEContext;

struct FGEContext
{
    GLFWwindow* window;
    const char* name;
    int width;
    int height;
};

FGEContext* FGEContextNew (char* in_title, int in_width, int in_height);
FGEError FGEContextDelete (FGEContext* in_context);

FGEBool FGEContextShouldClose (FGEContext* in_context);
FGEBool FGEContextIsFocused (FGEContext* in_context);

#ifndef FGE_WINDOW_TITLE
    #define FGE_WINDOW_TITLE "FLING GAME ENGINE"
#endif

#ifndef FGE_WINDOW_SIZE
    #define FGE_WINDOW_SIZE 800
#endif

#endif