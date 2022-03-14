#include "vao.h"
#include "vbo.h"


GLuint VAO_initialize() {

    GLuint ID;

    // Create array object for single element
    // MUST BE DONE BEFORE VBO
    glGenVertexArrays(1, &ID);

    return ID;
}

int VAO_linkVBO(GLuint VBO, GLuint layout) {

    // Bind VBO
    VBO_bind(VBO);

    // Configure VAO
    // Position, number of vertices, type, no, size, ?
    glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Enable VAO
    glEnableVertexAttribArray(layout);

    // Unbind VBO
    VBO_unbind();
}

int VAO_bind(GLuint ID) {
    glBindVertexArray(ID);
    return 0;
}

int VAO_unbind() {
    glBindVertexArray(0);
    return 0;
}

int VAO_delete(GLuint ID) {
    glDeleteVertexArrays(1, &ID);
    return 0;
}
