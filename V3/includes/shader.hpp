#pragma once

#include <string>

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

namespace FGE
{
    class Shader
    {
    public:
        /**
         * @brief Construct a new Shader object
         * 
         * @param vertex_path String path to the vertex shader file.
         * @param fragment_path String path to the fragment shader file.
         */
        Shader (std::string vertex_path, std::string fragment_path);

        /**
         * @brief Destroy the Shader object
         * 
         */
        ~Shader ();

        /**
         * @brief Bind the OpenGL object representing the shader.
         * 
         */
        void Use ();

        /**
         * @brief Unbind the OpenGL object representing the shader;
         * 
         */
        void Cancel ();

        /**
         * @brief Upload a 4x4 matrix as uniform to the shader.
         * 
         * @param key The name of the uniform.
         * @param matrix The matrix to upload.
         */
        void UploadMatrix4 (std::string key, glm::mat4& matrix);

        /**
         * @brief Upload an int as uniform to the shader.
         * 
         * @param key The name of the uniform.
         * @param i The int to upload.
         */
        void Upload1i (std::string key, int i);

    private:
        unsigned int m_program;
        unsigned int m_vertex_shader;
        unsigned int m_fragment_shader;

        std::string m_vertex_path;
        std::string m_fragment_path;
        std::string m_vertex_source;
        std::string m_fragment_source;

        /**
         * @brief Load shader source codes from file.
         * 
         * @param vertex_path The path to the vertex shader.
         * @param fragment_path The path to the fragment shader.
         * @return int: Returns 0 if unsuccesful and 1 otherwise.
         */
        int Load (std::string vertex_path, std::string fragment_path);

        /**
         * @brief Compile the shader source code.
         * 
         * @return int Returns 0 if unsuccesful and 1 otherwise.
         */
        int Compile ();

    };
}