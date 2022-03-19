#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "statemanager.h"

#include "shaders.h"
#include "vbo.h"
#include "ebo.h"
#include "vao.h"

// Debuggig macros
#define PRINT_VAR(X) printf(#X " is %d at address %p\n", X, &X);
#define SWAP(A, B) A ^= B ^= A ^= B;

#define WIDTH 800
#define HEIGHT 600

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

    // Vertices coordinates
    GLfloat vertices[] =
    { //               COORDINATES                  /     COLORS           //
        -0.5f, -0.5f * (float) sqrt(3) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f, // Lower left corner
         0.5f, -0.5f * (float) sqrt(3) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f, // Lower right corner
         0.0f,  0.5f * (float) sqrt(3) * 2 / 3, 0.0f,     1.0f, 0.6f,  0.32f, // Upper corner
        -0.25f, 0.5f * (float) sqrt(3) * 1 / 6, 0.0f,     0.9f, 0.45f, 0.17f, // Inner left
         0.25f, 0.5f * (float) sqrt(3) * 1 / 6, 0.0f,     0.9f, 0.45f, 0.17f, // Inner right
         0.0f, -0.5f * (float) sqrt(3) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f  // Inner down
    };

    GLuint indices[] =
    {
        0, 3, 5, // Lower left triangle
        3, 2, 4, // Lower right triangle
        5, 4, 1 // Upper triangle
    };

    // Create shaders
    // Pass absolute path for now
    GLuint shaderID = SHADERS_initialize("default.vert", "default.frag");

    // Create vertex array object reference (allows to switch between VBOs)
    GLuint VAO = VAO_initialize();
    VAO_bind(VAO);

    // Create buffer object reference
    GLuint VBO = VBO_initialize(vertices, sizeof(vertices));
    // Create index buffer object reference
    GLuint EBO = EBO_initialize(indices, sizeof(indices));

    // Links VBO to VAO
    VAO_linkAttrib(VBO, 0, 3, GL_FLOAT, 6 * sizeof(float), (void *) 0);
    // Offset is half of total size (position + color) to go to color
    VAO_linkAttrib(VBO, 1, 3, GL_FLOAT, 6 * sizeof(float), (void *)(3 * sizeof(float)));

    // Unbind all to prevent accidentally modifying them
    VAO_unbind();
    VBO_unbind();
    EBO_unbind();

    GLuint uniformID = glGetUniformLocation(shaderID, "scale");


    // Event loop
    while (!glfwWindowShouldClose(window)) {
        // Set color to blue
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Execute previous command
        glClear(GL_COLOR_BUFFER_BIT);

        // Run shader program
        SHADERS_activate(shaderID);
        glUniform1f(uniformID, 1.f);

        // Select which VAO to use
        VAO_bind(VAO);

        // Actual draw function
        // Primitive, index, number of vertices
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        // Show next buffer
        glfwSwapBuffers(window);

        // Catch events
        glfwPollEvents();
    }

    // Delete VAO, VBO, EBO and shaders
    SHADERS_delete(shaderID);
    VAO_delete(VAO);
    VBO_delete(VBO);
    EBO_delete(EBO);

    // Free window and free memory of window
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
