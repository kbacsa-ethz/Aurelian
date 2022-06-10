#include <stdio.h>
#include <stdlib.h>
#include <shaders.h>

#include "utils.h"

#define ERROR_LOG_LENGTH 1024


GLuint SHADERS_initialize(char *vertexFilename, char *fragmentFilename) {

    char *vertexShaderSource = UTILS_readfile(vertexFilename);
    char *fragmentShaderSource = UTILS_readfile(fragmentFilename);

    // ADD CONDITION IF NULL THEN QUIT
    // printf("vertexShaderSource = %s\n", vertexShaderSource);
    // printf("fragmentShaderSource = %s\n", fragmentShaderSource);

    // Create vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Load shader source
    glShaderSource(vertexShader, 1, (const char **) &vertexShaderSource, NULL);

     // Compile source that was just passed
    glCompileShader(vertexShader);

    // Check error log
    SHADERS_compileErrors(vertexShader, "VERTEX");

    // Repeat for fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const char **) &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    SHADERS_compileErrors(fragmentShader, "FRAGMENT");

    // Wrap up shaders in shader program
    GLuint ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    SHADERS_compileErrors(ID, "PROGRAM");

    // Delete individual shaders since they have been copied to the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return ID;
}

int SHADERS_activate(GLuint ID) {
    glUseProgram(ID);
    return 0;
}


int SHADERS_delete(GLuint ID) {
    glDeleteProgram(ID);
    return 0;
}

int SHADERS_compileErrors(unsigned int shader, const char *type) {
    char infolog[ERROR_LOG_LENGTH];
    GLint hasCompiled;
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE) {
            glGetShaderInfoLog(shader, ERROR_LOG_LENGTH, NULL, infolog);
            printf("SHADER_COMPILATION_ERROR for: %s at %d\n", type, shader);
        }
    }
    else {
        glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE) {
            glGetShaderInfoLog(shader, ERROR_LOG_LENGTH, NULL, infolog);
            printf("SHADER_LINKING_ERROR for: %s at %d\n", type, shader);
        }
    }
}
