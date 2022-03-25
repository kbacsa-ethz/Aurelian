#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/euler.h>
#include <cglm/io.h>

#include "statemanager.h"

#include "shaders.h"
#include "vbo.h"
#include "ebo.h"
#include "vao.h"
#include "texture.h"

#define DEBUG 1
#define CGLM_DEFINE_PRINTS 1

// Debuggig macros
#define PRINT_VAR(X) printf(#X " is %d at address %p\n", X, &X);
#define SWAP(A, B) A ^= B ^= A ^= B;

#define WIDTH 800
#define HEIGHT 800

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
    { //     COORDINATES     /        COLORS      /   TexCoord  //
        -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
         0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
         0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
         0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
    };

    // Indices for vertices order
    GLuint indices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
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
    VAO_linkAttrib(VBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *) 0);
    // Offset is half of total size (position + color) to go to color
    VAO_linkAttrib(VBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    // Texture coordinates
    VAO_linkAttrib(VBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float)));

    // Unbind all to prevent accidentally modifying them
    VAO_unbind();
    VBO_unbind();
    EBO_unbind();

    // Get entry point to shader uniform
    GLuint uniformID = glGetUniformLocation(shaderID, "scale");

    // Initialize texture (read image into texture bank)
    GLuint textureID = TEXTURE_initialize("lenna.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);

    // Activate uniform (mapping function)
    TEXTURE_texUnit(shaderID, "tex0", 0);

    // rotation variable
    float rotation = 0.0f;
    double prevTime = glfwGetTime();

    // Enable depth testing (what to draw on top)
    glEnable(GL_DEPTH_TEST);

    // Event loop
    while (!glfwWindowShouldClose(window)) {
        // Set color to blue
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Execute previous command (clear depth buffer bit as well)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Run shader program
        SHADERS_activate(shaderID);

        double crntTime = glfwGetTime();

        if ((crntTime - prevTime) >= 1 / 60) {
            rotation += 0.5f;
            prevTime = crntTime;
        }

        // init model, view and proj matrix with identity
        mat4 model, view, proj;
        glm_mat4_identity(model);
        glm_mat4_identity(view);
        glm_mat4_identity(proj);

        //Move back and up
        glm_translate(view, (vec3){0.0f, -0.5f, -2.0f});
        // Rotate for 3D effect (y axis)
        glm_rotate(model, glm_rad(rotation), (vec3){0.0f, 1.0f, 0.0f});
        // Field of view, aspect ratio, nearest z, furthest z
        glm_perspective(glm_rad(45.0f), (float)(WIDTH/HEIGHT), 0.1f, 100.0f, proj);

        int modelLoc = glGetUniformLocation(shaderID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model[0]);
        int viewLoc = glGetUniformLocation(shaderID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view[0]);
        int projLoc = glGetUniformLocation(shaderID, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, proj[0]);

        // Run uniform (must be done after program activation)
        glUniform1f(uniformID, 0.5f);

        TEXTURE_bind(GL_TEXTURE_2D, textureID); // bind to texture reference

        // Select which VAO to use
        VAO_bind(VAO);

        // Actual draw function
        // Primitive, index, number of vertices
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

        // Show next buffer
        glfwSwapBuffers(window);

        // Catch events
        glfwPollEvents();
    }

    // Delete VAO, VBO, EBO and shaders and textures
    SHADERS_delete(shaderID);
    VAO_delete(VAO);
    VBO_delete(VBO);
    EBO_delete(EBO);
    TEXTURE_delete(textureID);

    // Free window and free memory of window
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
