#include <math.h>
#include "camera.h"


int CAMERA_initialize(Camera *camera, int width, int height, vec3 position) {
    for(int i = 0; i < KEYBOARD_SIZE; i++) {
        camera -> activeKeys[i] = false;
    }

    glm_mat4_copy(GLM_MAT4_IDENTITY, camera -> cameraMatrix);
    camera -> width = width;
    camera -> height = height;
    camera -> zoom = 0;
    camera -> speed = 0.1f;
    camera -> speedZoom = 1.0f;
    camera -> sensitivity = 100.f;
    glm_vec3_copy(position, camera -> position);
    glm_vec3_copy((vec3){0.0f, -0.64f, -0.77f}, camera -> orientation);
    glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera -> up);
    return 0;
}

int CAMERA_updateMatrix(Camera *camera, float FOVdeg, float nearPlane, float farPlane) {
    // init model, view and proj matrix with identity
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    mat4 proj = GLM_MAT4_IDENTITY_INIT;

    vec3 center;
    glm_vec3_add(camera -> position, camera -> orientation, center);
    glm_lookat(camera -> position, center, camera -> up, view);

    // Field of view, aspect ratio, nearest z, furthest z
    glm_perspective(glm_rad(FOVdeg), (float)(camera -> width / camera -> height), nearPlane, farPlane, proj);
    glm_mat4_mul(proj, view, camera -> cameraMatrix);
}

int CAMERA_matrix(Camera *camera, GLuint shaderID, const char *uniform) {
    glUniformMatrix4fv(glGetUniformLocation(shaderID, uniform), 1, GL_FALSE, (float *) (camera -> cameraMatrix));
    return 0;
}

void CAMERA_keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
    Camera *camera = glfwGetWindowUserPointer(window);
    camera -> activeKeys[key] = action != GLFW_RELEASE;
}

void CAMERA_zoom(GLFWwindow* window, double xoffset, double yoffset) {
    Camera *camera = glfwGetWindowUserPointer(window);

    // Check not equal to 0
    if (fabs(yoffset) > 0.00001) {
        if (yoffset > 0) {
            camera -> zoom = 1;
        }
        else {
            camera -> zoom = -1;
        }
    }
    else {
        camera -> zoom = 0;
    }
}

void CAMERA_inputs(Camera *camera) {

    vec3 delta = GLM_VEC3_ZERO_INIT;

    // Handle key inputs
    if (camera -> activeKeys[GLFW_KEY_W]) {
        delta[2] = 1.0f;
        glm_vec3_scale(delta, camera -> speed, delta);
        glm_vec3_sub(camera -> position, delta, camera -> position);
        glm_vec3_zero(delta);
    }
    if (camera -> activeKeys[GLFW_KEY_A]) {
        delta[0] = 1.0f;
        glm_vec3_scale(delta, camera -> speed, delta);
        glm_vec3_sub(camera -> position, delta, camera -> position);
        glm_vec3_zero(delta);
    }
    if (camera -> activeKeys[GLFW_KEY_S]) {
        delta[2] = 1.0f;
        glm_vec3_scale(delta, camera -> speed, delta);
        glm_vec3_add(camera -> position, delta, camera -> position);
        glm_vec3_zero(delta);
    }
    if (camera -> activeKeys[GLFW_KEY_D]) {
        delta[0] = 1.0f;
        glm_vec3_scale(delta, camera -> speed, delta);
        glm_vec3_add(camera -> position, delta, camera -> position);
        glm_vec3_zero(delta);
    }

    for(int i = 0; i++; i < KEYBOARD_SIZE) {
        camera -> activeKeys[i] = false;
    }

    // Handle zoom
    if (camera -> zoom) {
        glm_vec3_copy(camera -> orientation, delta);
        glm_vec3_scale(delta, camera -> speedZoom, delta);

        if (camera -> zoom > 0) {
            glm_vec3_add(camera -> position, delta, camera -> position);
        }
        else {
            glm_vec3_sub(camera -> position, delta, camera -> position);
        }

        glm_vec3_zero(delta);
        camera -> zoom = 0; // reset here since mouse callback is slow
    }
}

int CAMERA_delete(Camera *camera) {
    free(camera);
    return 0;
}
