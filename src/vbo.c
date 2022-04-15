#include "vbo.h"


GLuint VBO_initialize(Vertices *vertices) {

    GLuint ID;

    // Create buffer object
    glGenBuffers(1, &ID);

    // Attach to buffer array
    glBindBuffer(GL_ARRAY_BUFFER, ID);

    // Load vertices (STATIC means that vertices cannot be modified)
    size_t totalSize = vertices -> sizePositions + vertices -> sizeColors + vertices -> sizeNormals + vertices -> sizeTextUVs;
    glBufferData(GL_ARRAY_BUFFER, totalSize, NULL, GL_STATIC_DRAW);

    // Load vertex data into sub-arrays
    if (vertices -> positions) {
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices -> sizePositions, vertices -> positions);
    }
    // TODO Raise error if no positions provided ?

    if (vertices -> colors) {
        glBufferSubData(GL_ARRAY_BUFFER, vertices -> sizePositions, vertices -> sizeColors, vertices -> colors);
    }

    if (vertices -> normals) {
        glBufferSubData(GL_ARRAY_BUFFER, vertices -> sizePositions + vertices -> sizeColors, vertices -> sizeNormals, vertices -> normals);
    }

    if (vertices -> textUVs) {
        glBufferSubData(GL_ARRAY_BUFFER, vertices -> sizePositions + vertices -> sizeColors + vertices -> sizeNormals, vertices -> sizeTextUVs, vertices -> textUVs);
    }

    return ID;
}

int VBO_bind(GLuint ID) {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    return 0;
}

int VBO_unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return 0;
}

int VBO_delete(GLuint ID) {
    glDeleteBuffers(1, &ID);
    return 0;
}
