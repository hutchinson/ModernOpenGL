#include <iostream>
#include <cmath>

// OpenGL Extension Manager
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "Shader.h"

const GLint WIDTH = 800;
const GLint HEIGHT = 600;

int main(int argc, const char** argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGL", nullptr, nullptr);
    if(window == nullptr)
    {
        std::cerr << "Failed to create window." << std::endl;
        glfwTerminate();
        return -1;
    }

    // This returns the actual underlying DPI
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    std::cout << "Screen Width " << screenWidth << "x" << screenHeight << std::endl;

    glfwMakeContextCurrent(window);

    // Allow us to use newer OpenGL features.
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialise OpenGL Extension Wrangler (GLEW)" << std::endl;
        return -1;
    }

    // Setup OpenGL viewport
    glViewport(0, 0, screenWidth, screenHeight);

    // Setup the shaders
    gl::Shader multiColorShader("SimpleVShader.glsl", "MultiColourFragShader.glsl");
    gl::Shader yellowShader("SimpleVShader.glsl", "YellowTriangleFragShader.glsl");

    GLfloat verticies[] = {
        // positions        // colours
        -0.75, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,         // bottom left
        0.0f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,        // bottom right
        -0.375f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,         // middle
        0.75, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,         // bottom left
        0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,         // bottom right
        0.375f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f         // middle.
    };

    // Create Vertex Buffer Objects one to store each of our triangles.
    // The VBO actually stores our data the VAO stores the vertex attribute
    // configuration that's applied to it.
    GLuint trianglesVBOs[2];
    GLuint trianglesVAOs[2];

    glGenBuffers(2, trianglesVBOs);
    glGenVertexArrays(2, trianglesVAOs);

    // Load each vertex buffer and store its configuration in the vertex array
    // object.
    for(int triangle = 0; triangle < 2; ++triangle)
    {
        // Record the subsequent vertex attribute configuration.
        glBindVertexArray(trianglesVAOs[triangle]);

        // Copy data to the GPU
        glBindBuffer(GL_ARRAY_BUFFER, trianglesVBOs[triangle]);

        GLfloat* startOfArray = verticies + ((6 * 3) * triangle);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticies) / 2, startOfArray, GL_STATIC_DRAW);

        // Configure the vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        // Unbind this vertex array.
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    // Unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set up the game loop...
    while( !glfwWindowShouldClose(window) )
    {
        glfwPollEvents();

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Load the configuration stored in the vertex array.
        for(int triangle = 0; triangle < 2; ++triangle)
        {
            if(triangle == 0)
            {
                // Setup our uniforms
                float timeValue = glfwGetTime();
                float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
                multiColorShader.use();

                // Push a value to the shader uniform
                //glUniform4f(colorUniform, 0.0f, greenValue, 0.0f, 1.0f);
            }
            else
            {
                yellowShader.use();
            }

            glBindVertexArray(trianglesVAOs[triangle]);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        // Unbind the array...
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(2, trianglesVAOs);
    glDeleteBuffers(2, trianglesVBOs);

    glfwTerminate();
    return 0;
}
