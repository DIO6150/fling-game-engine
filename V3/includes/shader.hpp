#pragma once

#include <string>

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

namespace FGE
{
    class Shader
    {
        unsigned int m_program;
        unsigned int m_vertex_shader;
        unsigned int m_fragment_shader;

        std::string m_vertex_path;
        std::string m_fragment_path;
        std::string m_vertex_source;
        std::string m_fragment_source;

        int Load (std::string vertex_path, std::string fragment_path);
        int Compile ();

    public:
        Shader (std::string vertex_path, std::string fragment_path);

        ~Shader ()
        {

        }

        void Create (std::string vertex_source, std::string fragment_source);
        void Delete ();

        void Use ()
        {
            glUseProgram(m_program);
        }

        void Cancel ()
        {
            glUseProgram(0);
        }

        void UploadMatrix4 (std::string key, glm::mat4& matrix)
        {
            // create a cache for uniform locations
            int location;

            location = glGetUniformLocation(m_program, key.c_str());
            glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
        }
    };
}