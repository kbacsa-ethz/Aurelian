#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "statemanager.h"
#include "shaders.h"
#include "map.h"
#include "map_generator.h"

#include <cglm/cglm.h>

// Debuggig macros
#define PRINT_VAR(X) printf(#X " is %d at address %p\n", X, &X);
#define SWAP(A, B) A ^= B ^= A ^= B;

//#define WIDTH 800
//#define HEIGHT 600

const GLuint width = 800;
const GLuint height = 600;

int main(int argc, char *argv[]) {

  // Initialize GLFW library
  glfwInit();

  // Indicate which version is used
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  // Use core (no backward compatibility)
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create window
  GLFWwindow *window = glfwCreateWindow(width, height, "Game Window", NULL, NULL);

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
  glViewport(0, 0, width, height);

  Map sample_map = get_sample_map(10, 10, -0.9f, -0.9f, 0.15f);

  GLfloat* vertices = get_map_vertices(sample_map);
  GLuint* indices = get_map_indices(sample_map);


// Vertices coordinates //               COORDINATES                  /     COLORS           //
//  GLfloat vertices[] =
//      {-0.5f, -0.5f * sqrt((float)(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f, // Lower left corner
//      0.5f, -0.5f * sqrt((float)(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f, // Lower right corner
//      0.0f,  0.5f * sqrt((float)(3)) * 2 / 3, 0.0f,     1.0f, 0.6f,  0.32f, // Upper corner
//      -0.25f, 0.5f * sqrt((float)(3)) * 1 / 6, 0.0f,     0.9f, 0.45f, 0.17f, // Inner left
//      0.25f, 0.5f * sqrt((float)(3)) * 1 / 6, 0.0f,     0.9f, 0.45f, 0.17f, // Inner right
//      0.0f, -0.5f * sqrt((float)(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f  // Inner down
//      };

//  GLuint indices[] =
//      {
//          0, 3, 5, // Lower left triangle
//          3, 2, 4, // Lower right triangle
//          5, 4, 1 // Upper triangle
//      };

//  GLfloat vertices[] =
//      {0.9f, 0.0f, 0.0f,     1.0f, 0.0f,  0.0f,
//       0.0f, 0.9f, 0.0f,     0.0f, 1.0f,  0.0f,
//       0.0f,  0.0f, 0.9f,     0.0f, 0.0f,  1.0f,
//       0.0f, 0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
//      };

//  GLfloat vertices[] =
//      {0.9f, 0.0f, -0.9f,     1.0f, 0.0f,  0.0f,
//       0.0f, 0.9f, 0.0f,     0.0f, 1.0f,  0.0f,
//       0.0f,  0.0f, 0.9f,     0.0f, 0.0f,  1.0f,
//       0.0f, 0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
//      };
//
//  GLuint indices[] =
//      {
//          0, 1, 2,
//          0, 1, 3,
//          0, 2, 3,
//          1, 2, 3
//      };


  // Create shaders
  // Pass absolute path for now
  GLuint shaderID = SHADERS_initialize(
//                "/home/user/moloc2/resource/shaders/default.vert",
//                "/home/user/moloc2/resource/shaders/default.frag"
              "/home/sbeuret/Documents/Aurelian/Aurelian/resource/shaders/default.vert",
              "/home/sbeuret/Documents/Aurelian/Aurelian/resource/shaders/default.frag"
              );

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
//  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBufferData(GL_ARRAY_BUFFER, 3 * sample_map.size_x * sample_map.size_z * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 2 * (sample_map.size_x - 1) * (sample_map.size_z - 1) * sizeof(GLint), indices, GL_STATIC_DRAW);

  // Configure VAO
  // Position, number of vertices, type, no, size, ?
//  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
//  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3*sizeof(GLfloat)));

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);

  // Enable VAO
  glEnableVertexAttribArray(0);
//  glEnableVertexAttribArray(1);

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

  glEnable(GL_DEPTH_TEST);

  // Gets ID of uniform called "scale"
//  GLuint uniID = glGetUniformLocation(shaderID, "scale");

  float rotation = 0.0f;
  double prevTime = glfwGetTime();

//  vec3 position = {0.0f, 0.0f, 2.0f};
//  vec3 orientation ={0.0f, 0.0f, -1.0f};
//  vec3 up = {0.0f, 1.0f, 0.0f};

//  vec3 position = {0.0f, 1.0f, -0.5f};
//  vec3 orientation ={0.0f, -0.5f, -0.2f};
//  vec3 up = {0.0f, 1.0f, 0.0f};

  vec3 position = {0.0f, 1.0f, 0.0f};
  vec3 orientation ={0.0f, -0.5f, -0.2f};
  vec3 up = {0.0f, 1.0f, 0.0f};


  float fov_deg = 45;
  float near_plane = 0.1f;
  float far_plane = 100.0f;

  float speed = 0.1f;

  // Event loop
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // test the keys
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
      position[2] = position[2] - speed;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
      position[2] = position[2] + speed;
    }
    // test the keys
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
      position[0] = position[0] + speed;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
      position[0] = position[0] - speed;
    }

    // Run shader program
    SHADERS_activate(shaderID);

    // configure the camera
    mat4 view, projection, full_matrix;
    glm_mat4_identity(view);
    glm_mat4_identity(projection);
    vec3 vec_local_out;

    glm_vec3_add(position, orientation, vec_local_out);
    glm_lookat(position, vec_local_out, up, view);
    glm_perspective(glm_rad(fov_deg),((float) width)/((float)height), near_plane, far_plane, projection);

    glm_mat4_mul(projection, view, full_matrix);
    int full_loc = glGetUniformLocation(shaderID, "camMatrix");
    glUniformMatrix4fv(full_loc, 1, GL_FALSE, &(full_matrix[0][0]));


//  // define the matrices
//    mat4 model, view, proj;

//    // initialize the matrices with identity
//    glm_mat4_identity(model);
//    glm_mat4_identity(view);
//    glm_mat4_identity(proj);
//
//    // define the first matrix as a rotation matrix
//    vec3 rotation_axis = {0.0f, 1.0f, 0.0f};
//    glm_rotate(model, glm_rad(rotation), rotation_axis);
//    vec3 translation_vector = {0.0f, -0.5f, -2.0f};
//    glm_translate(view, translation_vector);
//    glm_perspective(glm_rad(45.0f),((float) width)/height, 0.1f, 100.0f, proj);
//
//    // Outputs the matrices into the Vertex Shader
//    int modelLoc = glGetUniformLocation(shaderID, "model");
//    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &(model[0][0]));
//    int viewLoc = glGetUniformLocation(shaderID, "view");
//    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &(view[0][0]));
//    int projLoc = glGetUniformLocation(shaderID, "proj");
//    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &(proj[0][0]));


//    glUniform1f(uniID, 0.5f);

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
  SHADERS_delete(shaderID);

  free_map(&sample_map);

  // Free window and free memory of window
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
