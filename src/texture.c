#include <stb/stb_image.h>
#include "texture.h"
#include "shaders.h"


GLuint TEXTURE_initialize(const char *image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) {

    GLuint ID;
    // Generate textures (number of pictures, reference)
    glGenTextures(1, &ID);
    glActiveTexture(slot); // activate texture "bank"
    glBindTexture(texType, ID); // bind to texture reference

    // Choose type of interpolation (here use linear interpolation)
    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Choose type of repetition
    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT); // Repeat along S-axis
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT); // Repeat along T-axis

    // Texture loading
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(1);
    // Loads image into character array
    unsigned char *bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

    if (bytes)
    {
        glTexImage2D(texType, 0, GL_RGB, widthImg, heightImg, 0, format, pixelType, bytes);
        glGenerateMipmap(texType);
    }
    else
    {
        printf("Failed to load texture.\n");
    }

    // Free memory once texture is loaded
    stbi_image_free(bytes);


    // Unbind texture to avoid modifying it
    glBindTexture(texType, 0);

    return ID;
}

int TEXTURE_texUnit(GLuint shaderID, const char *uniform, GLuint unit) {
    // Use texture uniform
    GLuint tex0Uniform = glGetUniformLocation(shaderID, uniform);
    SHADERS_activate(shaderID);
    glUniform1i(tex0Uniform, unit);
    return 0;
}

int TEXTURE_bind(GLenum texType, GLuint ID) {
    glBindTexture(texType, ID);
    return 0;
}

int TEXTURE_unbind(GLenum texType) {
    glBindTexture(texType, 0);
    return 0;
}

int TEXTURE_delete(GLuint ID) {
    glDeleteTextures(1, &ID);
    return 0;
}
