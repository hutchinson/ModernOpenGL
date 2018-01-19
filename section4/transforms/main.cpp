#include <iostream>
#include <cmath>

// OpenGL Extension Manager
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include "Shader.h"

const GLint WIDTH = 800;
const GLint HEIGHT = 600;
GLfloat mixLevel = 0.2f;
GLfloat step = 0.1f;

unsigned char* loadTexture(const char* filePath, int* width, int* height, int* numChannel)
{
    stbi_set_flip_vertically_on_load(true);
    return stbi_load(filePath, width, height, numChannel, 0);
}

void configureTexture(const char* filePath, GLuint* textureID)
{
    // Load the texture.
    int width, height, numChannels;
    unsigned char* data = loadTexture(filePath, &width, &height, &numChannels);
    if(!data)
    {
        std::cerr << "Unable to load texture.";
        return;
    }

    glGenTextures(1, textureID);

    // Set it as the target for our subsequent calls.
    glBindTexture(GL_TEXTURE_2D, *textureID);

    // Set it as the target for our subsequent calls.
    glBindTexture(GL_TEXTURE_2D, *textureID);

    // Set up texture wrapping and filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Now we're going to transfer that texture data previously loaded to the CPU
    glTexImage2D(GL_TEXTURE_2D,     // target
                 0,                 // mip map level
                 GL_RGB,            // format we'll store the pixels in OpenGL
                 width, height,     // texture width & height
                 0,                 // legacy
                 numChannels == 3 ? GL_RGB : GL_RGBA,            // source image format
                 GL_UNSIGNED_BYTE,  // data type of the source image.
                 data);             // pointer to image data.
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free the image memory in RAM (its now on the GPU)
    stbi_image_free(data);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action != GLFW_RELEASE)
        return;

    if(key == GLFW_KEY_UP)
        mixLevel += step;
    else if(key == GLFW_KEY_DOWN)
        mixLevel -= step;
}

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

    glfwSetKeyCallback(window, key_callback);

    // This returns the actual underlying DPI
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

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

    GLuint texture1ID, texture2ID;
    configureTexture("container.jpg", &texture1ID);
    configureTexture("awesomeface.png", &texture2ID);

    // Setup the shaders
    gl::Shader multiColorShader("SimpleVShader.glsl", "MultiColourFragShader.glsl");
    multiColorShader.use();
    multiColorShader.setFloat("mixLevel", 0.2f);
    multiColorShader.setInt("outTexture", 0);
    multiColorShader.setInt("ourTexture2", 1);

    GLfloat verticies[] = {
        // positions          // colours       // texture coordinates
        0.5f,   0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f,         // top right
        0.5f,  -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,         // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,         // bottom left
        -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f          // top left
    };

    GLint indicies[] = {
        0, 1, 3,
        1, 2, 3
    };

    // Create Vertex Buffer Objects one to store each of our triangles.
    // The VBO actually stores our data the VAO stores the vertex attribute
    // configuration that's applied to it.
    GLuint vbo;
    GLuint ebo;
    GLuint vao;

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);

    // Load the vertex buffer and store its configuration in the vertex array
    glBindVertexArray(vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    // Copy data to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

    // Configure the vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), /* stride */ (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), /* stride */ (GLvoid*)(3 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), /* stride */ (GLvoid*)(6 * sizeof(GLfloat)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Unbind this vertex array.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

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

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1ID);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2ID);

        // Load the configuration stored in the vertex array.
        multiColorShader.use();
        multiColorShader.setFloat("mixLevel", mixLevel);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

        // Unbind the array...
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return 0;
}
