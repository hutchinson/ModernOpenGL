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

    // Set uniforms for this shader
    void setBool(const std::string& name, GLboolean value) const;
    void setInt(const std::string& name, GLint value) const;
    void setFloat(const std::string& name, GLfloat value) const;

private:
    // Read the contents of the file specified by filePath and return it.
    const std::string&& readFile(const char* filePath);
};

}   // namespace gl

#endif
