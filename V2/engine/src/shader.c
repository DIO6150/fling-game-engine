#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "glad/glad.h"

FGEShader* FGEShaderLoad (char* in_vertex_path, char* in_fragment_path)
{
    FGEShader* shader = malloc (sizeof (FGEShader));

    assert (shader);

    FILE* fvertex;
    FILE* ffragment;
    
    char vertex_source[1000];
    char fragment_source[1000];

    char line[256];

    fvertex = fopen (in_vertex_path, "r");

    if (!fvertex)
    {
        printf ("%s(%s) : File doesn't exist : %s.\n", __func__, __FILE__, in_vertex_path);
        return (0);
    }

    ffragment = fopen (in_fragment_path, "r");

    if (!ffragment)
    {
        fclose (fvertex);
        printf ("%s(%s) : File doesn't exist : %s.\n", __func__, __FILE__, in_fragment_path);
        return (0);
    }

    while (fgets(line, sizeof(line), fvertex))
    {
        strcat (vertex_source, line);
    }

    while (fgets(line, sizeof(line), ffragment))
    {
        strcat (fragment_source, line);
    }

    fclose (fvertex);
    fclose (ffragment);

    if (!FGEShaderNew (vertex_source, fragment_source))
    {
        printf("%s(%s) : Could'nt create the shader.\n", __func__, __FILE__);
        return (0);
    }

    shader->vertex_path = in_vertex_path;
    shader->fragment_path = in_fragment_path;

    return (shader);
}

FGEShader* FGEShaderNew (char* in_vertex_source, char* in_fragment_source)
{
    FGEShader* shader = (FGEShader*) malloc(sizeof(FGEShader));

    shader->vertex_path = NULL;
    shader->fragment_path = NULL;

    shader->vertex_source = in_vertex_source;
    shader->fragment_source = in_fragment_source;
    
    shader->program = 0;
    shader->vertex_shader = 0;
    shader->fragment_shader = 0;

    if (!FGEShaderCompile (shader))
    {
        free (shader);
        printf ("%s(%s) : Could'nt compile shader, aborting.\n", __func__, __FILE__);
        return (NULL);
    }

    return (shader);
}

FGEError FGEShaderDelete (FGEShader* in_shader)
{
    if (!in_shader)
    {
        printf("%s(%s) : FGEShader is NULL.\n", __func__, __FILE__);
        return (0);
    }

    glDeleteShader(in_shader->vertex_shader);
    glDeleteShader(in_shader->fragment_shader);
    glDeleteProgram(in_shader->program);

    free(in_shader);

    return (1);
}

FGEError FGEShaderCompile (FGEShader* in_shader)
{

    if (!in_shader)
    {
        printf("%s(%s) : FGEShader is NULL :\n", __func__, __FILE__);

        return (FGE_E_SHADER_COMPILE);
    }

    if (!in_shader->vertex_source)
    {
        printf("%s(%s) : Vertex Source is NULL :\n", __func__, __FILE__);

        return (FGE_E_SHADER_COMPILE);
    }

    if (!in_shader->fragment_source)
    {
        printf("%s(%s) : Fragment Source is NULL :\n", __func__, __FILE__);

        return (FGE_E_SHADER_COMPILE);
    }

    char info_log[1024];
    int success;
    unsigned int vertex_shader;
    unsigned int fragment_shader;

    const char* vertex_source = in_shader->vertex_source;

    const char* fragment_source = in_shader->fragment_source;

    // Vertex FGEShader Step

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);
    
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        printf("%s(%s) : Error when compiling vertex shader :\n%s\n", __func__, __FILE__, info_log);

        return (FGE_E_SHADER_COMPILE);
    }

    // Fragment FGEShader Step

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        printf("%s(%s) : Error when compiling fragment shader :\n%s\n", __func__, __FILE__, info_log);

        return (FGE_E_SHADER_COMPILE);
    }

    // FGEShader Program Step

    in_shader->program = glCreateProgram();

    glAttachShader(in_shader->program, vertex_shader);
    glAttachShader(in_shader->program, fragment_shader);
    glLinkProgram(in_shader->program);

    glGetProgramiv(in_shader->program, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(in_shader->program, 512, NULL, info_log);
        printf("%s(%s) : Error when compiling shader program :\n%s\n", __func__, __FILE__, info_log);

        return (FGE_E_SHADER_COMPILE);
    }

    return (FGE_E_SUCCESS);
}

FGEError FGEShaderUse (FGEShader* in_shader)
{
    if (!in_shader)
    {
        printf("%s(%s) : FGEShader is NULL.\n", __func__, __FILE__);
        return (0);
    }

    glUseProgram(in_shader->program);
    return (FGE_E_SUCCESS);
}

FGEError FGEShaderCancel ()
{
    glUseProgram(0);
    return (FGE_E_SUCCESS);
}