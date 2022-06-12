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
#include "map_graphics.h"


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

    // intialize the map
    Map main_map = MAP_BASIC_GENERATOR_get_sample_map(120, 120, -2.5f, -2.5f, 0.06f);

    // initialize a test mesh
    GLfloat test_mesh_positions[] = {-0.1f, 0.7f, 0.1f,
                                     -0.1f, 0.7f, -0.1f,
                                     0.1f, 0.7f, -0.1f,
                                     -0.1f, 0.9f, 0.1f};

    GLfloat test_mesh_normals[] = {-1.f, 0.f, 0.f,
                                   0.f, -1.f, 0.f,
                                   0.f, 0.f, -1.f,
                                   1.f, 0.f, 0.f};

    GLfloat test_mesh_textUVs[] = {1.f, 0.5f,
                                   2.f, 0.6f,
                                   9.f, 0.7f,
                                   1.f, 1.f};

    GLuint test_mesh_indices[] = {0, 1, 2, 0, 1, 3, 0, 2, 3, 1, 2, 3};

    PositionsArray test_mesh_positions_array = {.positions_ptr=&test_mesh_positions,
                                                .size_positions=sizeof(test_mesh_positions)};
    NormalsArray test_mesh_normals_array = {.normals_ptr=&test_mesh_normals,
                                            .size_normals=sizeof(test_mesh_normals)};
    TextUVsArray test_mesh_textUVs_array = {.textUVs_ptr=&test_mesh_textUVs,
                                            .size_textUVs=sizeof(test_mesh_textUVs)};
    IndicesArray test_mesh_indices_array = {.indices_ptr=&test_mesh_indices,
                                            .size_indices=sizeof(test_mesh_indices)};




    // Create shaders
    // Pass absolute path for now
    GLuint map_shaderID = SHADERS_initialize("map.vert", "map.frag");

    GLuint mesh_shaderID = SHADERS_initialize("default.vert", "default.frag");

    // Initialize texture (read image into texture bank)
    int nTextures = 2;
    GLuint *textures = malloc(nTextures * sizeof(GLuint));
    *textures = TEXTURE_initialize("lenna.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
    *(textures + 1) = TEXTURE_initialize("lennaSpec.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);

    MapMesh* map_mesh_ptr = malloc(sizeof(MapMesh));
    MAP_GRAPHICS_get_map_mesh(map_mesh_ptr, main_map, map_shaderID, textures, nTextures,0.5, 0.5);

    Mesh* test_mesh_ptr = malloc(sizeof(Mesh));
    MESH_initialize(test_mesh_ptr, test_mesh_positions_array, test_mesh_normals_array, test_mesh_textUVs_array,
                    test_mesh_indices_array, textures, nTextures, mesh_shaderID);

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



    // initialize the shader for the map
    SHADERS_activate(map_shaderID);
    glUniformMatrix4fv(glGetUniformLocation(map_shaderID, "model"), 1, GL_FALSE, (float *) pyramidModel);
    glUniform4f(glGetUniformLocation(map_shaderID, "lightColor"), lightColor[0], lightColor[1], lightColor[2],
                lightColor[3]);
    glUniform3f(glGetUniformLocation(map_shaderID, "lightPos"), lightPos[0], lightPos[1], lightPos[2]);

    // add the modulo for the map texture
    glUniform2f(glGetUniformLocation(map_shaderID, "texSizeVal"), 0.2f, 0.2f);

    // initialize the shader for the mesh
    SHADERS_activate(mesh_shaderID);
    glUniformMatrix4fv(glGetUniformLocation(mesh_shaderID, "model"), 1, GL_FALSE, (float *) pyramidModel);
    glUniform4f(glGetUniformLocation(mesh_shaderID, "lightColor"), lightColor[0], lightColor[1], lightColor[2],
                lightColor[3]);
    glUniform3f(glGetUniformLocation(mesh_shaderID, "lightPos"), lightPos[0], lightPos[1], lightPos[2]);




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

        MAP_GRAPHICS_draw_map_mesh(map_mesh_ptr, gameCamera);
        MESH_draw(test_mesh_ptr, gameCamera);

        // Show next buffer
        glfwSwapBuffers(window);

        // Catch events
        glfwPollEvents();
    }

    // Delete VAO, VBO, EBO and shaders and textures
    SHADERS_delete(map_shaderID);
    SHADERS_delete(mesh_shaderID);
    MAP_GRAPHICS_delete_map_mesh(map_mesh_ptr);
    MESH_delete(test_mesh_ptr);

    // This only works because there's only one texture
    TEXTURE_delete(*textures);
    CAMERA_delete(gameCamera);


    // free the map
    Map_free(&main_map);


    // Free window and free memory of window
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
