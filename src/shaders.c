#include <stdio.h>
#include <stdlib.h>
#include <shaders.h>


char *SHADERS_readfile(const char *filename) {

    // from https://stackoverflow.com/questions/3747086/reading-the-whole-text-file-into-a-char-array-in-c
    char *shaderpointer;
    long length;
    FILE *f = fopen (filename, "rb");

    if (f) {
      fseek (f, 0, SEEK_END);
      length = ftell (f);
      fseek (f, 0, SEEK_SET);
      shaderpointer = (char*)malloc ((length+1)*sizeof(char));
      if (shaderpointer) {
        fread (shaderpointer, sizeof(char), length, f);
        *(shaderpointer+length) = '\0'; // Prevent garbage at the end of string.
      }

      fclose (f);
    }

    return shaderpointer;

}


int SHADERS_initialize(char *vertexFilename, char *fragmentFilename) {

    char *vertexShaderSource = SHADERS_readfile(vertexFilename);
    char *fragmentShaderSource = SHADERS_readfile(fragmentFilename);

    // ADD CONDITION IF NULL THEN QUIT
    printf("vertexShaderSource = %s\n", vertexShaderSource);
    printf("fragmentShaderSource = %s\n", fragmentShaderSource);

    // Create vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Load shader source
    glShaderSource(vertexShader, 1, (const char **) &vertexShaderSource, NULL);

     // Compile source that was just passed
    glCompileShader(vertexShader);

    // Repeat for fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const char **) &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Wrap up shaders in shader program
    GLuint ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

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
