#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "shaders.h"

#define KEYBOARD_SIZE 512

typedef struct {
    bool activeKeys[KEYBOARD_SIZE];
    vec3 position;
    vec3 orientation;
    vec3 up;
    float speed;
    float speedZoom;
    float sensitivity;
    int width;
    int height;
    int zoom;
} Camera;

int CAMERA_initialize(Camera *camera, int width, int height, vec3 position);
int CAMERA_matrix(Camera *camera, float FOVdeg, float nearPlane, float farPlane, GLuint shaderID, const char *uniform);
void CAMERA_keyboard(GLFWwindow *window, int key, int scancode, int action, int mods);
void CAMERA_inputs(Camera *camera);
void CAMERA_zoom(GLFWwindow* window, double xoffset, double yoffset);
int CAMERA_delete(Camera *camera);

#endif // CAMERA_H
