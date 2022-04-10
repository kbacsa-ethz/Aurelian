#include "vbo.h"


GLuint VBO_initialize(Vertex *vertices) {

    GLuint ID;

    // Create buffer object
    glGenBuffers(1, &ID);

    // Attach to buffer array
    glBindBuffer(GL_ARRAY_BUFFER, ID);

    // Load vertices (STATIC means that vertices cannot be modified)
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

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
