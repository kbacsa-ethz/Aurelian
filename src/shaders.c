#include <stdio.h>
#include <stdlib.h>
#include <shaders.h>


void *SHADERS_readfile(char **shaderpointer, const char *filename) {

    // from https://stackoverflow.com/questions/3747086/reading-the-whole-text-file-into-a-char-array-in-c
    long length;
    FILE *f = fopen (filename, "rb");

    if (f)
    {
      fseek (f, 0, SEEK_END);
      length = ftell (f);
      fseek (f, 0, SEEK_SET);
      *shaderpointer = (char*)malloc ((length+1)*sizeof(char));
      if (*shaderpointer)
      {
        fread (*shaderpointer, sizeof(char), length, f);
        *(*shaderpointer+length) = '\0';
      }

      fclose (f);
    }
}


int SHADERS_initialize(Shaders *shader, char *vertexFilename, char *fragmentFilename) {
    SHADERS_readfile(&(shader -> vertexShaderSource), vertexFilename);
    SHADERS_readfile(&(shader -> fragmentShaderSource), fragmentFilename);

    // ADD CONDITION IF NULL THEN QUIT
    printf("vertexShaderSource = %s\n", shader -> vertexShaderSource);
    printf("fragmentShaderSource = %s\n", shader -> fragmentShaderSource);

    // Create vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Load shader source
    glShaderSource(vertexShader, 1, (const char **) &(shader -> vertexShaderSource), NULL);

     // Compile source that was just passed
    glCompileShader(vertexShader);

    // Repeat for fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const char **) &(shader -> fragmentShaderSource), NULL);
    glCompileShader(fragmentShader);

    // Wrap up shaders in shader program
    shader -> ID = glCreateProgram();
    glAttachShader(shader -> ID, vertexShader);
    glAttachShader(shader -> ID, fragmentShader);
    glLinkProgram(shader -> ID);

    // Delete individual shaders since they have been copied to the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return 0;
}


int SHADERS_activate(Shaders *shader) {
    glUseProgram(shader -> ID);
    return 0;
}


int SHADERS_delete(Shaders *shader) {
    glDeleteProgram(shader -> ID);
    return 0;
}
