#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "statemanager.h"

// Debuggig macros
#define PRINT_VAR(X) printf(#X " is %d at address %p\n", X, &X);
#define SWAP(A, B) A ^= B ^= A ^= B;

#define WIDTH 800
#define HEIGHT 600

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";


int main(int argc, char *argv[]) {

    // Initialize GLFW library
    glfwInit();

    // Indicate which version is used
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Use core (no backward compatibility)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Game Window", NULL, NULL);

    // Quit if failed to create
    if (window == NULL) {
       printf("Failed to create GLFW window.\n");
       glfwTerminate();
       return -1;
    }

    // Introduce window to current context
    glfwMakeContextCurrent(window);

    // Load GLAD library
    gladLoadGL();

    // Select rendeing area
    glViewport(0, 0, WIDTH, HEIGHT);

    // Vertices of triangle
    GLfloat vertices[] =
    {
        -0.5f, -0.5f * (float)sqrt(3)/ 3, 0.0f, // Lower left corner
        0.5f, -0.5f * (float)sqrt(3)/ 3, 0.0f, // Lower right corner
        0.0f, 0.5f * (float)sqrt(3) * 2 / 3, 0.0f, // Upper corner
        -0.5f / 2, 0.5f * (float)sqrt(3) / 6, 0.0f, // Inner left
        0.5f / 2, 0.5f * (float)sqrt(3) / 6, 0.0f, // Inner right
        0.0f, -0.5f * (float)sqrt(3) / 3, 0.0f, // Inner right
    };

    GLuint indices[] =
    {
        0, 3, 5, // Lower left triangle
        3, 2, 4, // Lower right triangle
        5, 4, 1 // Upper triangle
    };

    // Create vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Load shader source
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

     // Compile source that was just passed
    glCompileShader(vertexShader);

    // Repeat for fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Wrap up shaders in shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Delete individual shaders since they have been copied to the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Create vertex array object reference (allows to switch between VBOs)
    GLuint VAO;
    // Create buffer object reference
    GLuint VBO;
    // Create index buffer object reference
    GLuint EBO;

    // Create array object for single element
    // MUST BE DONE BEFORE VBO
    glGenVertexArrays(1, &VAO);

    // Create buffer object for single element
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Attach to buffer array
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // Load vertices (STATIC means that vertices cannot be modified)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configure VAO
    // Position, number of vertices, type, no, size, ?
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    // Enable VAO
    glEnableVertexAttribArray(0);

    // Bind to zero (prevents change ?)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // MUST BE DONE AFTER VERTEX ARRAY

    // Set color to blue
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    // Execute previous command
    glClear(GL_COLOR_BUFFER_BIT);
    // Show next buffer
    glfwSwapBuffers(window);

    // Event loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Run shader program
        glUseProgram(shaderProgram);

        // Select which VAO to use
        glBindVertexArray(VAO);

        // Actual draw function
        // Primitie, index, number of vertices
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);

        // Catch events
        glfwPollEvents();
    }

    // Delete VAO and VBO
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // Free window and free memory of window
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
