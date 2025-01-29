#include "shader.hpp"

#include <iostream>
#include <fstream>
#include <string>

FGE::Shader::Shader (std::string vertex_path, std::string fragment_path)
{
    if (!Load (vertex_path, fragment_path)) return;
    if (!Compile ()) return;

    printf ("Shader Created Successfully\n");
}

FGE::Shader::~Shader ()
{

}

int FGE::Shader::Load (std::string vertex_path, std::string fragment_path)
{
    
    std::string vertex_source;
    std::string fragment_source;

    std::string line;

    std::ifstream fvertex (vertex_path);
    std::ifstream ffragment (fragment_path);

    if (!(fvertex.is_open () && ffragment.is_open ()))
    {
        fvertex.close ();
        ffragment.close ();
        printf ("Error when loading file");
        return (0);
    }

    while (getline (fvertex, line))
    {
        vertex_source += line + '\n';
    }
    fvertex.close ();


    while (getline (ffragment, line))
    {
        fragment_source += line + '\n';
    }
    ffragment.close ();

    m_vertex_path.assign (vertex_path);
    m_fragment_path.assign (fragment_path);

    m_vertex_source.assign (vertex_source);
    m_fragment_source.assign (fragment_source);

    return (1);
}

int FGE::Shader::Compile ()
{
    char info_log[1024];
    int success;

    // Vertex Shader Step
    m_vertex_shader = glCreateShader (GL_VERTEX_SHADER);

    const char* vertex_source = m_vertex_source.c_str ();
    const char* fragment_source = m_fragment_source.c_str ();

    glShaderSource (m_vertex_shader, 1, &vertex_source, NULL);
    glCompileShader (m_vertex_shader);
    
    glGetShaderiv (m_vertex_shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog (m_vertex_shader, 512, NULL, info_log);
        printf("%s(%s) : Error when compiling vertex shader :\n%s\n", __func__, __FILE__, info_log);

        return (0);
    }

    // Fragment Shader Step
    m_fragment_shader = glCreateShader (GL_FRAGMENT_SHADER);

    glShaderSource (m_fragment_shader, 1, &fragment_source, NULL);
    glCompileShader (m_fragment_shader);

    glGetShaderiv (m_fragment_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog (m_fragment_shader, 512, NULL, info_log);
        printf("%s(%s) : Error when compiling fragment shader :\n%s\n", __func__, __FILE__, info_log);

        return (0);
    }

    // Shader Program Step

    m_program = glCreateProgram ();

    glAttachShader (m_program, m_vertex_shader);
    glAttachShader (m_program, m_fragment_shader);
    glLinkProgram (m_program);

    glGetProgramiv (m_program, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog (m_program, 512, NULL, info_log);
        printf("%s(%s) : Error when compiling shader program :\n%s\n", __func__, __FILE__, info_log);

        return (0);
    }

    return (1);
}

void FGE::Shader::Use ()
{
    glUseProgram (m_program);
}

void FGE::Shader::Cancel ()
{
    glUseProgram (0);
}

void FGE::Shader::UploadMatrix4 (std::string key, glm::mat4& matrix)
{
    // TODO : create a cache for uniform locations
    int location;

    location = glGetUniformLocation (m_program, key.c_str ());
    glUniformMatrix4fv (location, 1, GL_FALSE, &matrix[0][0]);
}

void FGE::Shader::Upload1i (std::string key, int i)
{
    int location;

    location = glGetUniformLocation (m_program, key.c_str ());
    glUniform1i (location, i);
}