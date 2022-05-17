#include "ebo.h"

// TODO: add pointer rather than a pointer of pointers

GLuint EBO_initialize(IndicesArray indices_array){

    GLuint ID;

    // Create buffer object
    glGenBuffers(1, &ID);

    // Attach to buffer array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);

    // Load vertices (STATIC means that vertices cannot be modified)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_array.size_indices, indices_array.indices_ptr,
                 GL_STATIC_DRAW);

    return ID;
}

int EBO_bind(GLuint ID) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    return 0;
}

int EBO_unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return 0;
}

int EBO_delete(GLuint ID) {
    glDeleteBuffers(1, &ID);
    return 0;
}
