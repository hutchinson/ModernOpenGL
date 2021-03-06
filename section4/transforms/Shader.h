#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <GL/glew.h>

namespace gl
{

class Shader
{
public:
    Shader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);

    // Disable assignment, copy and move constructors
    Shader(const Shader& rhs) = delete;
    Shader& operator=(const Shader& rhs) = delete;

    Shader(const Shader&& rhs) = delete;
    Shader& operator=(const Shader&& rhs) = delete;

    ~Shader();

    // Activate the shader for use.
    void use();

    const GLint id() const { return m_shaderProgram; }

    // Set uniforms for this shader
    void setBool(const std::string& name, GLboolean value);
    void setInt(const std::string& name, GLint value);
    void setFloat(const std::string& name, GLfloat value);

private:
    // Read the contents of the file specified by filePath and return it.
    const std::string&& readFile(const char* filePath);

    GLint m_shaderProgram;
};

}   // namespace gl

#endif
