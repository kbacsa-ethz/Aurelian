#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/euler.h>

#include "statemanager.h"

#include "shaders.h"
#include "vbo.h"
#include "ebo.h"
#include "vao.h"
#include "texture.h"
#include "camera.h"
#include "mesh.h"
#include "map.h"
#include "map_basic_generator.h"


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


    Map main_map = MAP_BASIC_GENERATOR_get_sample_map(350, 350, -2.5f, -2.5f, 0.02f);

    Vertices *mapVertices;
    mapVertices = (Vertices *) malloc(sizeof(Vertices));

    mapVertices->positions = MAP_get_positions(main_map);
    mapVertices->sizePositions = MAP_get_positions_size(main_map);

    mapVertices->normals = MAP_get_normals(main_map);
    mapVertices->sizeNormals = MAP_get_normals_size(main_map);

    mapVertices->textUVs = MAP_get_text_uvs(main_map, 0.5, 0.5);
    mapVertices->sizeTextUVs = MAP_get_text_uvs_size(main_map);

    mapVertices->colors = MAP_get_colors(main_map);
    mapVertices->sizeColors = MAP_get_colors_size(main_map);


    GLuint *indices = MAP_get_indices(main_map);
    size_t size_indices = MAP_get_indices_size(main_map);

    // Light source (cube)
    GLfloat lightPositions[] =
        { //     COORDINATES     //
            -0.1f, 0.7f, 0.1f,
            -0.1f, 0.7f, -0.1f,
            0.1f, 0.7f, -0.1f,
            0.1f, 0.7f, 0.1f,
            -0.1f, 0.9f, 0.1f,
            -0.1f, 0.9f, -0.1f,
            0.1f, 0.9f, -0.1f,
            0.1f, 0.9f, 0.1f
        };

    GLuint lightIndices[] =
        {
            0, 1, 2,
            0, 2, 3,
            0, 4, 7,
            0, 7, 3,
            3, 7, 6,
            3, 6, 2,
            2, 6, 5,
            2, 5, 1,
            1, 5, 4,
            1, 4, 0,
            4, 5, 6,
            4, 6, 7
        };

    Vertices *lightVertices;
    lightVertices = (Vertices *) malloc(sizeof(Vertices));
    lightVertices->positions = malloc(8 * 3 * sizeof(float));
    for (int i = 0; i < 8; i++) {
        *(lightVertices->positions + 3 * i) = lightPositions[3 * i];
        *(lightVertices->positions + 3 * i + 1) = lightPositions[3 * i + 1];
        *(lightVertices->positions + 3 * i + 2) = lightPositions[3 * i + 2];
    }

    lightVertices->colors = NULL;
    lightVertices->normals = NULL;
    lightVertices->textUVs = NULL;

    lightVertices->sizePositions = 8 * 3 * sizeof(float);
    lightVertices->sizeColors = 0;
    lightVertices->sizeNormals = 0;
    lightVertices->sizeTextUVs = 0;

    // Create shaders
    // Pass absolute path for now
    GLuint shaderID = SHADERS_initialize("default.vert", "default.frag");

    // Initialize texture (read image into texture bank)
    int nTextures = 1;
    GLuint *textures = malloc(nTextures * sizeof(GLuint));
    *textures = TEXTURE_initialize("lenna.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);

    Mesh *pyramidMesh = malloc(sizeof(Mesh));
    MESH_initialize(pyramidMesh, mapVertices, indices, textures, MAP_get_indices_size(main_map), nTextures);

    // Shader for light cube
    GLuint lightShaderID = SHADERS_initialize("light.vert", "light.frag");

    Mesh *lightMesh = malloc(sizeof(Mesh));
    MESH_initialize(lightMesh, lightVertices, lightIndices, textures, sizeof(lightIndices), 0);

    vec4 lightColor;
    glm_vec3_copy((vec4) {1.0f, 1.0f, 0.9f, 1.0f}, lightColor);
    vec3 lightPos;
    glm_vec3_copy((vec3) {1.5f, 1.5f, 1.5f}, lightPos);
    mat4 lightModel = GLM_MAT4_IDENTITY_INIT;
    glm_translate(lightModel, lightPos);

    vec3 pyramidPos;
    glm_vec3_copy((vec3) {0.5f, 0.5f, 0.5f}, pyramidPos);
    mat4 pyramidModel = GLM_MAT4_IDENTITY_INIT;
    glm_translate(pyramidModel, pyramidPos);

    SHADERS_activate(lightShaderID);
    glUniformMatrix4fv(glGetUniformLocation(lightShaderID, "model"), 1, GL_FALSE, (float *) lightModel);
    glUniform4f(glGetUniformLocation(lightShaderID, "lightColor"), lightColor[0], lightColor[1], lightColor[2],
                lightColor[3]);

    SHADERS_activate(shaderID);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, (float *) pyramidModel);
    glUniform4f(glGetUniformLocation(shaderID, "lightColor"), lightColor[0], lightColor[1], lightColor[2],
                lightColor[3]);
    glUniform3f(glGetUniformLocation(shaderID, "lightPos"), lightPos[0], lightPos[1], lightPos[2]);

    // Enable depth testing (what to draw on top)
    glEnable(GL_DEPTH_TEST);

    Camera *gameCamera;
    gameCamera = (Camera *) malloc(sizeof(Camera));
    CAMERA_initialize(gameCamera, WIDTH, HEIGHT, (vec3) {0.53f, 3.0f, 4.0f});

    // Define callbacks for inputs
    glfwSetKeyCallback(window, CAMERA_keyboard);
    glfwSetScrollCallback(window, CAMERA_zoom);

    // This function allows us to pass data from the callback
    glfwSetWindowUserPointer(window, gameCamera);

    // Event loop
    while (!glfwWindowShouldClose(window)) {

        // Set color to blue
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Execute previous command (clear depth buffer bit as well)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update position based on key inputs
        CAMERA_inputs(gameCamera);
        CAMERA_updateMatrix(gameCamera, 45.0f, 0.1f, 100.0f);

        MESH_draw(pyramidMesh, shaderID, gameCamera);
        MESH_draw(lightMesh, lightShaderID, gameCamera);

        // Show next buffer
        glfwSwapBuffers(window);

        // Catch events
        glfwPollEvents();
    }

    // Delete VAO, VBO, EBO and shaders and textures
    SHADERS_delete(shaderID);
    MESH_delete(pyramidMesh);
    SHADERS_delete(lightShaderID);
    MESH_delete(lightMesh);

    // This only works because there's only one texture
    TEXTURE_delete(*textures);
    CAMERA_delete(gameCamera);

    // Free static memory
    free(mapVertices->positions);
    free(mapVertices->colors);
    free(mapVertices->normals);
    free(mapVertices->textUVs);
    free(mapVertices);

    // free the map
    Map_free(&main_map);

    free(lightVertices->positions);
    free(lightVertices->colors);
    free(lightVertices->normals);
    free(lightVertices->textUVs);
    free(lightVertices);

    // Free window and free memory of window
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
