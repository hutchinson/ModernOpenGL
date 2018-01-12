#include "Shader.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace gl
{

constexpr size_t kCompileLogBufferSize = 512;

Shader::Shader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath)
    : m_shaderProgram(-1)
{
    std::ifstream vertexShaderInputStream, fragmentShaderInputStream;

    // Configure to throw exceptions
    vertexShaderInputStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderInputStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::string vertexCode, fragmentCode;
    try
    {
        vertexShaderInputStream.open(vertexShaderFilePath, std::ios_base::in);
        fragmentShaderInputStream.open(fragmentShaderFilePath, std::ios_base::in);

        std::stringstream vShaderSS, fShaderSS;
        vShaderSS << vertexShaderInputStream.rdbuf();
        fShaderSS << fragmentShaderInputStream.rdbuf();

        // Close the files
        vertexShaderInputStream.close();
        fragmentShaderInputStream.close();

        vertexCode = vShaderSS.str();
        fragmentCode = fShaderSS.str();
    }
    catch(const std::ios_base::failure& exception)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << exception.what() << std::endl;
    }

    // Now compile the shaders.
    const char* vShaderSourceCStr = vertexCode.c_str();
    const char* fShaderSourceCStr = fragmentCode.c_str();

    int success = 0;
    char infoLog[kCompileLogBufferSize];

    GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderSourceCStr, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        memset(infoLog, 0, kCompileLogBufferSize);
        glGetShaderInfoLog(vertexShader, kCompileLogBufferSize, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderSourceCStr, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        memset(infoLog, 0, kCompileLogBufferSize);
        glGetShaderInfoLog(vertexShader, kCompileLogBufferSize, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link to create a shader program.
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);

    // Check for linking errors
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        memset(infoLog, 0, kCompileLogBufferSize);
        glGetProgramInfoLog(m_shaderProgram, kCompileLogBufferSize, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Clean up the shader resources...
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{

}

void Shader::use()
{
    glUseProgram(m_shaderProgram);
}

void Shader::setBool(const std::string& name, GLboolean value)
{
    GLint uniformLocation = glGetUniformLocation(m_shaderProgram, name.c_str());
    glUniform1i(uniformLocation, (GLint)value);
}

void Shader::setInt(const std::string &name, GLint value)
{
    GLint uniformLocation = glGetUniformLocation(m_shaderProgram, name.c_str());
    glUniform1i(uniformLocation, (GLint)value);
}

void Shader::setFloat(const std::string& name, GLfloat value)
{
    GLint uniformLocation = glGetUniformLocation(m_shaderProgram, name.c_str());
    glUniform1f(uniformLocation, value);
}

}   //  namespace gl
