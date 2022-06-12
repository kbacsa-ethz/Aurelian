#include "vbo.h"

// TODO: Add pointer rather than pointer on pointers


GLuint VBO_initialize_Positions_Normals(PositionsArray positions_array, NormalsArray normals_array){

    GLuint ID;

    // Create buffer object
    glGenBuffers(1, &ID);

    // Attach to buffer array
    glBindBuffer(GL_ARRAY_BUFFER, ID);

    // Load vertices (STATIC means that vertices cannot be modified)
    size_t total_size = positions_array.size_positions + normals_array.size_normals;
    glBufferData(GL_ARRAY_BUFFER, total_size, NULL, GL_STATIC_DRAW);

    // Load vertex data into sub-arrays
    if (positions_array.positions_ptr) {
        glBufferSubData(GL_ARRAY_BUFFER, 0, positions_array.size_positions, positions_array.positions_ptr);
    }

    if (normals_array.normals_ptr) {
        glBufferSubData(GL_ARRAY_BUFFER, positions_array.size_positions, normals_array.size_normals,
                        normals_array.normals_ptr);
    }

    return ID;
}

GLuint VBO_initialize_Positions_Normals_TextUVs(PositionsArray positions_array, NormalsArray normals_array,
                                                TextUVsArray textUVs_array){

    GLuint ID;

    // Create buffer object
    glGenBuffers(1, &ID);

    // Attach to buffer array
    glBindBuffer(GL_ARRAY_BUFFER, ID);

    // Load vertices (STATIC means that vertices cannot be modified)
    size_t total_size = positions_array.size_positions + normals_array.size_normals +
        textUVs_array.size_textUVs;
    glBufferData(GL_ARRAY_BUFFER, total_size, NULL, GL_STATIC_DRAW);

    // Load vertex data into sub-arrays
    if (positions_array.positions_ptr) {
        glBufferSubData(GL_ARRAY_BUFFER, 0, positions_array.size_positions, positions_array.positions_ptr);
    }

    if (normals_array.normals_ptr) {
        glBufferSubData(GL_ARRAY_BUFFER, positions_array.size_positions, normals_array.size_normals,
                        normals_array.normals_ptr);
    }

    if(textUVs_array.textUVs_ptr){
        glBufferSubData(GL_ARRAY_BUFFER, positions_array.size_positions + normals_array.size_normals,
                        textUVs_array.size_textUVs, textUVs_array.textUVs_ptr);
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
