#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our
{

    class ShaderProgram
    {

    private:
        // Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram()
        {
            // TODO: Done (Req 1) Create A shader program
            program = glCreateProgram();
        }
        ~ShaderProgram()
        {
            // TODO: Done (Req 1) Delete a shader program
            if (program != 0)
                glDeleteProgram(program);
        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use()
        {
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name)
        {
            // TODO: Done (Req 1) Return the location of the uniform with the given name
            return glGetUniformLocation(program, name.c_str());
        }

        void set(const std::string &uniform, GLfloat value)
        {
            // TODO: Done (Req 1) Send the given float value to the given uniform
            glUniform1f(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, GLuint value)
        {
            // TODO: Done (Req 1) Send the given unsigned integer value to the given uniform
            glUniform1ui(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, GLint value)
        {
            // TODO: Done (Req 1) Send the given integer value to the given uniform
            glUniform1i(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, glm::vec2 value)
        {
            // TODO: Done (Req 1) Send the given 2D vector value to the given uniform
            glUniform2f(getUniformLocation(uniform), value.x, value.y);
        }

        void set(const std::string &uniform, glm::vec3 value)
        {
            // TODO: Done (Req 1) Send the given 3D vector value to the given uniform
            glUniform3f(getUniformLocation(uniform), value.x, value.y, value.z);
        }

        void set(const std::string &uniform, glm::vec4 value)
        {
            // TODO: Done (Req 1) Send the given 4D vector value to the given uniform
            glUniform4f(getUniformLocation(uniform), value.x, value.y, value.z, value.w);
        }

        void set(const std::string &uniform, glm::mat4 matrix)
        {
            // TODO: Done (Req 1) Send the given matrix 4x4 value to the given uniform
            glUniformMatrix4fv(getUniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(matrix));
        }

        // TODO: Done (Req 1) Delete the copy constructor and assignment operator.
        // Question: Why do we delete the copy constructor and assignment operator?
        // The copy constructor and assignment operator are deleted to prevent multiple instances from sharing and trying to manage the same GPU resource.
        // This way if it was copied somewhere else it will be destroyed to prevent any issues.
        ShaderProgram(const ShaderProgram &) = delete;
        ShaderProgram &operator=(const ShaderProgram &) = delete;
    };

}

#endif