#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "shaders.h"

typedef struct {
    vec3 position;
    vec3 orientation;
    vec3 up;
    float speed;
    float sensitivity;
    int width;
    int height;
    bool firstClick;
} Camera;

int CAMERA_initialize(Camera *camera, int width, int height, vec3 position);
int CAMERA_matrix(Camera *camera, float FOVdeg, float nearPlane, float farPlane, GLuint shaderID, const char *uniform);
int CAMERA_inputs(Camera *camera, GLFWwindow* window);
int CAMERA_delete(Camera *camera);

#endif // CAMERA_H
