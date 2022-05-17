#include <stdio.h>

#include "mesh.h"
#include "shaders.h"
#include "camera.h"



int MESH_initialize(Mesh *mesh, PositionsArray positions_array, NormalsArray normals_array, TextUVsArray textUVs_array,
                    IndicesArray indices_array, GLuint *textureIDs_ptr, int nTextures, GLuint shaderID){

    // fill the mesh
    mesh->positions_array = positions_array;
    mesh->normals_array = normals_array;
    mesh->textUVs_array = textUVs_array;
    mesh->indices_array = indices_array;

    mesh->textureIDs_ptr = textureIDs_ptr;
    mesh->nTextures = nTextures;

    mesh->shaderID = shaderID;

    //create the buffers

    // Create vertex array object reference (allows to switch between VBOs)
    mesh -> VAO = VAO_initialize();
    VAO_bind(mesh -> VAO);

    // Create buffer object reference
    mesh -> VBO = VBO_initialize(mesh->positions_array, mesh->normals_array,
                                 mesh->textUVs_array);

    // Create index buffer object reference
    mesh -> EBO = EBO_initialize(mesh->indices_array);

    // Links VBO to VAO
    VAO_linkAttrib(mesh -> VBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void *) 0);

    // Normals
    VAO_linkAttrib(mesh -> VBO, 1, 3, GL_FLOAT, 3 * sizeof(float),
                   (void *)((mesh -> positions_array).size_positions));
    // Texture coordinates
    VAO_linkAttrib(mesh -> VBO, 2, 2, GL_FLOAT, 2 * sizeof(float),
                   (void *)(((mesh -> positions_array).size_positions) +
                            ((mesh -> normals_array).size_normals)));

    VAO_unbind();
    VBO_unbind();
    EBO_unbind();
}

int MESH_draw(Mesh *mesh_ptr, Camera *camera){

    VAO_bind(mesh_ptr -> VAO);

    for (unsigned int i = 0; i < mesh_ptr -> nTextures; i++) {
        // Activate uniform (mapping function)
        char string[4];
        snprintf(string, 4, "tex%d", i);
        TEXTURE_texUnit(mesh_ptr -> shaderID, "tex0", i);
        TEXTURE_bind(GL_TEXTURE_2D, *(mesh_ptr -> textureIDs_ptr+i)); // bind to texture reference
    }

    // Camera orientation and projection
    CAMERA_matrix(camera, mesh_ptr -> shaderID, "camMatrix");

    // Actual draw function
    // Primitive, index, number of vertices
    glDrawElements(GL_TRIANGLES, (mesh_ptr -> indices_array).size_indices / sizeof(GLuint), GL_UNSIGNED_INT, 0);
    return 0;
}

int MESH_delete(Mesh *mesh) {
    VAO_delete(mesh -> VAO);
    VBO_delete(mesh -> VBO);
    EBO_delete(mesh -> EBO);
    free(mesh);
    return 0;
}

//int MESH_initialize(Mesh *mesh, Vertices *verticesRef, GLuint *indices, GLuint *textureIDs, size_t indicesSize, int nTextures) {
//    mesh -> vertices = verticesRef;
//    mesh -> indices = indices;
//    mesh -> textureIDs = textureIDs;
//    mesh -> indicesSize = indicesSize;
//    mesh -> nTextures = nTextures;
//
//    // Create vertex array object reference (allows to switch between VBOs)
//    mesh -> VAO = VAO_initialize();
//    VAO_bind(mesh -> VAO);
//
//    // Create buffer object reference
//    mesh -> VBO = VBO_initialize(mesh -> vertices);
//    // Create index buffer object reference
//    mesh -> EBO = EBO_initialize(mesh -> indices, mesh -> indicesSize);
//
//    // Links VBO to VAO
//    VAO_linkAttrib(mesh -> VBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void *) 0);
//    // Offset is half of total size (position + color) to go to color
////    VAO_linkAttrib(mesh -> VBO, 1, 3, GL_FLOAT, 3 * sizeof(float), (void *)(mesh -> vertices -> sizePositions));
//    // Normals
//    VAO_linkAttrib(mesh -> VBO, 1, 3, GL_FLOAT, 3 * sizeof(float), (void *)(mesh -> vertices -> sizePositions));
//    // Texture coordinates
//    VAO_linkAttrib(mesh -> VBO, 2, 2, GL_FLOAT, 2 * sizeof(float), (void *)(mesh -> vertices -> sizePositions + mesh -> vertices -> sizeNormals));
//
//    // Unbind all to prevent accidentally modifying them
//    VAO_unbind();
//    VBO_unbind();
//    EBO_unbind();
//
//    return 0;
//}
//
//int MESH_draw(Mesh *mesh, GLuint shaderID, Camera *camera) {
//
//    SHADERS_activate(shaderID);
//    VAO_bind(mesh -> VAO);
//
//    for (unsigned int i = 0; i < mesh -> nTextures; i++) {
//        // Activate uniform (mapping function)
//        char string[4];
//        snprintf(string, 4, "tex%d", i);
//        TEXTURE_texUnit(shaderID, "tex0", i);
//        TEXTURE_bind(GL_TEXTURE_2D, *(mesh -> textureIDs+i)); // bind to texture reference
//    }
//
//    // Camera orientation and projection
//    CAMERA_matrix(camera, shaderID, "camMatrix");
//
//    // Actual draw function
//    // Primitive, index, number of vertices
//    glDrawElements(GL_TRIANGLES, mesh -> indicesSize / sizeof(int), GL_UNSIGNED_INT, 0);
//    return 0;
//}
//
//int MESH_delete(Mesh *mesh) {
//    VAO_delete(mesh -> VAO);
//    VBO_delete(mesh -> VBO);
//    EBO_delete(mesh -> EBO);
//    free(mesh);
//    return 0;
//}
