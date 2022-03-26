#include "camera.h"
#include <cglm/io.h>

int CAMERA_initialize(Camera *camera, int width, int height, vec3 position) {
    camera -> width = width;
    camera -> height = height;
    camera -> speed = 0.1f;
    camera -> sensitivity = 100.f;
    camera -> firstClick = true;
    glm_vec3_copy(position, camera -> position);
    glm_vec3_copy((vec3){0.0f ,0.0f, -1.0f}, camera -> orientation);
    glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera -> up);
    return 0;
}

int CAMERA_matrix(Camera *camera, float FOVdeg, float nearPlane, float farPlane, GLuint shaderID, const char *uniform) {
    // init model, view and proj matrix with identity
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    mat4 proj = GLM_MAT4_IDENTITY_INIT;
    mat4 finalTransform = GLM_MAT4_IDENTITY_INIT;

    vec3 center;
    glm_vec3_add(camera -> position, camera -> orientation, center);
    glm_lookat(camera -> position, center, camera -> up, view);

    // Field of view, aspect ratio, nearest z, furthest z
    glm_perspective(glm_rad(FOVdeg), (float)(camera -> width / camera -> height), nearPlane, farPlane, proj);
    glm_mat4_mul(proj, view, finalTransform);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, uniform), 1, GL_FALSE, (float *)finalTransform);

    return 0;
}

int CAMERA_inputs(Camera *camera, GLFWwindow* window) {

    vec3 delta = GLM_VEC3_ONE_INIT;

    // Handle key inputs
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        glm_vec3_scale(camera -> orientation, camera -> speed, delta);
        glm_vec3_add(camera -> position, delta, camera -> position);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        glm_vec3_crossn(camera -> orientation, camera -> up, delta);
        glm_vec3_scale(delta, camera -> speed, delta);
        glm_vec3_sub(camera -> position, delta, camera -> position);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        glm_vec3_scale(camera -> orientation, camera -> speed, delta);
        glm_vec3_sub(camera -> position, delta, camera -> position);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        glm_vec3_crossn(camera -> orientation, camera -> up, delta);
        glm_vec3_scale(delta, camera -> speed, delta);
        glm_vec3_add(camera -> position, delta, camera -> position);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        glm_vec3_scale(camera -> up, camera -> speed, delta);
        glm_vec3_add(camera -> position, delta, camera -> position);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        glm_vec3_scale(camera -> up, camera -> speed, delta);
        glm_vec3_sub(camera -> position, delta, camera -> position);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera -> speed = 0.4f;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        camera -> speed = 0.1f;
    }

    // Handles mouse inputs
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        // Hides mouse cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // Prevents camera from jumping on the first click
        if (camera -> firstClick) {
            glfwSetCursorPos(window, (camera -> width / 2), (camera -> height / 2));
            camera -> firstClick = false;
        }

        // Stores the coordinates of the cursor
        double mouseX;
        double mouseY;
        // Fetches the coordinates of the cursor
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
        // and then "transforms" them into degrees
        float rotX = camera -> sensitivity * (float)(mouseY - (camera -> height / 2)) / camera -> height;
        float rotY = camera -> sensitivity * (float)(mouseX - (camera -> width / 2)) / camera -> width;

        // Calculates upcoming vertical change in the Orientation
        vec3 newOrientation = GLM_VEC3_ONE_INIT;
        glm_vec3_copy(camera -> orientation, newOrientation);
        glm_vec3_crossn(camera -> orientation, camera -> up, delta);
        glm_vec3_rotate(newOrientation, glm_rad(-rotX), delta); // newOrientation has changed here

        // Decides whether or not the next vertical Orientation is legal or not
        if (abs(glm_vec3_angle(newOrientation, camera -> up) - glm_rad(90.0f)) <= glm_rad(85.0f)) {
            glm_vec3_copy(newOrientation, camera -> orientation);
        }

        // Rotates the Orientation left and right
        glm_vec3_rotate(camera -> orientation, glm_rad(-rotY), camera -> up);

        // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
        glfwSetCursorPos(window, (camera -> width / 2), (camera -> height / 2));
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        // Unhides cursor since camera is not looking around anymore
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Makes sure the next time the camera looks around it doesn't jump
        camera -> firstClick = true;
    }

    return 0;
}

int CAMERA_delete(Camera *camera) {
    free(camera);
    return 0;
}
