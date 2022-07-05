#include "model.h"
#include "mesh.h"
#include "mesh_array.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>


int MODEL_initialize(Model *model) {
    char *file = NULL;
    char *string = NULL;
    cJSON *element = NULL;

    model -> nTextures = 0;

    file = UTILS_readfile("Lantern.gltf");
    model -> modelJSON = cJSON_Parse(file);

    cJSON *buffers = cJSON_GetObjectItemCaseSensitive(model -> modelJSON, "buffers");
    cJSON_ArrayForEach(element, buffers) {
       cJSON *byteLengthJSON = cJSON_GetObjectItemCaseSensitive(element, "byteLength");
       unsigned int byteLength = byteLengthJSON -> valueint;
       cJSON *uriJSON = cJSON_GetObjectItemCaseSensitive(element, "uri");
       model -> data = malloc(byteLength * sizeof(char));
       FILE *ptr;
       ptr = fopen(uriJSON -> valuestring, "rb");
       fread(model -> data, byteLength, 1, ptr);
       fclose(ptr);
    }

    // get max number of meshes and allocate memory
    // FOR NOW WE ASSUME THAT THERE IS A SINGLE SCENE
    cJSON *scenesJSON = cJSON_GetObjectItemCaseSensitive(model -> modelJSON, "scenes");
    if (scenesJSON != NULL) {
         cJSON *sceneJSON = cJSON_GetArrayItem(scenesJSON, 0);
         cJSON *nodesJSON = cJSON_GetObjectItemCaseSensitive(sceneJSON, "nodes");
         if (nodesJSON != NULL) {
             cJSON_ArrayForEach(element, nodesJSON) {
                 model -> nMeshes = element -> valueint;
             }
         }
    }

    // assign memory to number of meshes
    model -> translation_mesh_array = malloc(model -> nMeshes * sizeof(vec3));
    model -> rotation_mesh_array = malloc(model -> nMeshes * sizeof(versor));
    model -> scale_mesh_array = malloc(model -> nMeshes * sizeof(vec3));
    model -> matrix_mesh_array = malloc(model -> nMeshes * sizeof(mat4));

    free(file);
    free(string);
    return 0;
}

floatVec *MODEL_getFloats(Model *model, cJSON *accessor) {

    // Get properties from the accessor
    cJSON *buffViewIndJSON = cJSON_GetObjectItemCaseSensitive(accessor, "bufferView");
    unsigned int buffViewInd;
    // default to 1 if value is missing
    if (buffViewIndJSON == NULL) buffViewInd = 1;
    else buffViewInd = buffViewIndJSON -> valueint;

    cJSON *accByteOffsetJSON = cJSON_GetObjectItemCaseSensitive(accessor, "byteOffset");
    unsigned int accByteOffset;
    // default to 0 if value is missing
    if (accByteOffsetJSON == NULL) accByteOffset = 0;
    else accByteOffset = accByteOffsetJSON -> valueint;

    cJSON *countJSON = cJSON_GetObjectItemCaseSensitive(accessor, "count");
    unsigned int count = countJSON -> valueint;

    cJSON *typeJSON = cJSON_GetObjectItemCaseSensitive(accessor, "type");

    // Get properties from the bufferView
    cJSON *buffViewsJSON = cJSON_GetObjectItemCaseSensitive(model -> modelJSON, "bufferViews");
    cJSON *buffViewJSON = cJSON_GetArrayItem(buffViewsJSON, buffViewInd);

    cJSON *byteOffsetJSON = cJSON_GetObjectItemCaseSensitive(buffViewJSON, "byteOffset");
    unsigned int byteOffset;
    if (byteOffsetJSON == NULL) byteOffset = 0;
    else byteOffset = byteOffsetJSON -> valueint;

    unsigned int numPerVert;
    if (!strcmp(typeJSON -> valuestring, "SCALAR")) numPerVert = 1;
    else if (!strcmp(typeJSON -> valuestring, "VEC2") ) numPerVert = 2;
    else if (!strcmp(typeJSON -> valuestring, "VEC3")) numPerVert = 3;
    else if (!strcmp(typeJSON -> valuestring, "VEC4")) numPerVert = 4;
    else return NULL; // TODO Implement error message

    // Go over all the bytes in the data at the correct place using the properties from above
    unsigned int beginningOfData = byteOffset + accByteOffset;
    unsigned int lengthOfData = count * 4 * numPerVert;

    floatVec *floatData = malloc(sizeof(floatVec));
    floatData -> vecData = malloc(lengthOfData * sizeof(float) / 4);
    for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i) {
        unsigned char bytes[4] = { model -> data[i++], model -> data[i++], model -> data[i++], model -> data[i++] };
        float value;
        memcpy(&value, bytes, sizeof(float));
        *(floatData -> vecData + floatData -> vecSize) = value;
        floatData -> vecSize++;
    }

    return floatData;
}

intVec *MODEL_getIndices(Model *model, cJSON *accessor) {

    // Get properties from the accessor
    cJSON *buffViewIndJSON = cJSON_GetObjectItemCaseSensitive(accessor, "bufferView");
    unsigned int buffViewInd;
    // default to 1 if value is missing
    if (buffViewIndJSON == NULL) buffViewInd = 1;
    else buffViewInd = buffViewIndJSON -> valueint;

    cJSON *accByteOffsetJSON = cJSON_GetObjectItemCaseSensitive(accessor, "byteOffset");
    unsigned int accByteOffset;
    // default to 0 if value is missing
    if (accByteOffsetJSON == NULL) accByteOffset = 0;
    else accByteOffset = accByteOffsetJSON -> valueint;

    cJSON *countJSON = cJSON_GetObjectItemCaseSensitive(accessor, "count");
    unsigned int count = countJSON -> valueint;

    cJSON *typeJSON = cJSON_GetObjectItemCaseSensitive(accessor, "componentType");

    // Get properties from the bufferView
    cJSON *buffViewsJSON = cJSON_GetObjectItemCaseSensitive(model -> modelJSON, "bufferViews");
    cJSON *buffViewJSON = cJSON_GetArrayItem(buffViewsJSON, buffViewInd);

    cJSON *byteOffsetJSON = cJSON_GetObjectItemCaseSensitive(buffViewJSON, "byteOffset");
    unsigned int byteOffset;
    if (byteOffsetJSON == NULL) byteOffset = 0;
    else byteOffset = byteOffsetJSON -> valueint;

    // Go over all the bytes in the data at the correct place using the properties from above
    unsigned int beginningOfData = byteOffset + accByteOffset;

    unsigned int lengthOfData = 0;
    if (typeJSON -> valueint == 5125) {
        lengthOfData = count * 4;
    }
    else {
        lengthOfData = count * 2;
    }

    printf("%d\n", count);
    intVec *intData = malloc(sizeof(intVec));
    intData -> vecData = malloc(count * 2 * sizeof(unsigned int));
    intData -> vecSize = 0;
#if 0
    if (typeJSON -> valueint == 5125) {
        for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i) {
            unsigned char bytes[4] = {model -> data[i++], model -> data[i++], model -> data[i++], model -> data[i++]};
            unsigned int value;
            memcpy(&value, bytes, sizeof(unsigned int));
            *(intData -> vecData + intData -> vecSize) = value;
            intData -> vecSize++;
        }
    }

    if (typeJSON -> valueint == 5123) {
        for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i) {
            unsigned char bytes[2] = {model -> data[i++], model -> data[i++]};
            unsigned short value;
            memcpy(&value, bytes, sizeof(unsigned short));
            *(intData -> vecData + intData -> vecSize) = (unsigned int)value;
            intData -> vecSize++;
        }
    }

    if (typeJSON -> valueint == 5122) {
        for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i) {
            unsigned char bytes[2] = {model -> data[i++], model -> data[i++]};
            short value;
            memcpy(&value, bytes, sizeof(short));
            *(intData -> vecData + intData -> vecSize) = (unsigned int)value;
            intData -> vecSize++;
        }
    }
#endif

    return intData;
}

int MODEL_getTextures(Model *model) {

    // get directory (here root path)

    // get images
    cJSON *imagesJSON = cJSON_GetObjectItemCaseSensitive(model -> modelJSON, "images");
    cJSON *imageJSON = NULL;

    // iterate through images
    cJSON_ArrayForEach(imageJSON, imagesJSON) {
        cJSON *uriJSON = cJSON_GetObjectItemCaseSensitive(imageJSON, "uri");
        char *string = NULL;
        string = cJSON_Print(uriJSON);
        printf("%s\n", string);

        // add two arrays to model
        // list of texture names
        // list of textures

        // iterate through loaded textures
        bool skip = false;
        // get number of loaded textures
        unsigned int loadedTexture = 0;
        for (unsigned int j = 0; j < model -> nTextures; j++) {
            // if name found
            // append loaded texture to texture list
            if (!strcmp(model -> textureNames + j, string)) {
                skip = false;
                break;
            }
        }

        if (!skip) {
            unsigned int currentTexture = model -> nTextures;
            realloc(model -> textures, (currentTexture + 1) * sizeof(GLuint));

            if (strstr(string, "baseColor") != NULL) {
                printf("Detected base color\n");
                *(model -> textures + currentTexture) = TEXTURE_initialize(string, GL_TEXTURE_2D, loadedTexture, GL_RGB, GL_UNSIGNED_BYTE);
            }
            // invert metallicRoughness to roughnessMetallic
            else if (strstr(string, "roughnessMetallic") != NULL) {
                printf("Detected base roughness\n");
                TEXTURE_initialize(string, GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
            }
            else {
                // raise error
                return 1;
            }

            realloc(model -> textureNames, (currentTexture + 1) * sizeof(char*));
            *(model -> textureNames + currentTexture) = (char*) malloc(256 * sizeof(char));
            strcpy(*(model -> textureNames + currentTexture), string);
            model -> nTextures++;
        }
     }
    return 0;
}


int MODEL_loadMesh(Model *model, unsigned int indMesh) {

    cJSON *meshesJSON = cJSON_GetObjectItemCaseSensitive(model -> modelJSON, "meshes");
    cJSON *meshJSON = cJSON_GetArrayItem(meshesJSON, indMesh);
    cJSON *primitivesJSON = cJSON_GetObjectItemCaseSensitive(meshJSON, "primitives");
    cJSON *primitiveJSON = cJSON_GetArrayItem(primitivesJSON, 0);
    cJSON *attributesJSON = cJSON_GetObjectItemCaseSensitive(primitiveJSON, "attributes");
    cJSON *positionJSON = cJSON_GetObjectItemCaseSensitive(attributesJSON, "POSITION");
    cJSON *normalJSON = cJSON_GetObjectItemCaseSensitive(attributesJSON, "NORMAL");
    cJSON *texcoord_0JSON = cJSON_GetObjectItemCaseSensitive(attributesJSON, "TEXCOORD_0");
    cJSON *indicesJSON = cJSON_GetObjectItemCaseSensitive(primitiveJSON, "indices");

    unsigned int posAccInd = positionJSON -> valueint;
    unsigned int normalAccInd = normalJSON -> valueint;
    unsigned int texAccInd = texcoord_0JSON -> valueint;
    unsigned int indAccInd = indicesJSON -> valueint;

    cJSON *accessorsJSON = cJSON_GetObjectItemCaseSensitive(model -> modelJSON, "accessors");

    cJSON *accessorJSON = cJSON_GetArrayItem(accessorsJSON, posAccInd);
    floatVec *posVec = MODEL_getFloats(model, accessorJSON);

    // convert to position array
    PositionsArray *position_array = malloc(sizeof(PositionsArray));
    position_array -> size_positions = posVec -> vecSize * sizeof(GLfloat);
    position_array -> positions_ptr = malloc(posVec -> vecSize * sizeof(GLfloat));
    for(int i = 0; i < posVec -> vecSize; i++) {
        *(position_array -> positions_ptr + i) = *(posVec -> vecData + i);
    }
    POSITIONS_ARRAY_free(position_array);
    free(posVec -> vecData);
    free(posVec);

    accessorJSON = cJSON_GetArrayItem(accessorsJSON, normalAccInd);
    floatVec *normalVec = MODEL_getFloats(model, accessorJSON);
    NormalsArray *normals_array = malloc(sizeof(NormalsArray));
    normals_array -> size_normals = normalVec -> vecSize * sizeof(GLfloat);
    normals_array-> normals_ptr = malloc(normalVec -> vecSize * sizeof(GLfloat));
    for(int i = 0; i < normalVec -> vecSize; i++) {
        *(normals_array -> normals_ptr + i) = *(normalVec -> vecData + i);
    }
    NORMALS_ARRAY_free(normals_array);
    free(normalVec -> vecData);
    free(normalVec);

    accessorJSON = cJSON_GetArrayItem(accessorsJSON, texAccInd);
    floatVec *texVec = MODEL_getFloats(model, accessorJSON);
    TextUVsArray *text_uv_array = malloc(sizeof(TextUVsArray));
    text_uv_array -> size_textUVs = texVec -> vecSize * sizeof(GLfloat);
    text_uv_array -> textUVs_ptr = malloc(texVec -> vecSize * sizeof(GLfloat));
    for(int i = 0; i < texVec -> vecSize; i++) {
        *(text_uv_array -> textUVs_ptr + i) = *(texVec -> vecData + i);
    }
    TEXTUVS_ARRAY_free(text_uv_array);
    free(texVec -> vecData);
    free(texVec);

    accessorJSON = cJSON_GetArrayItem(accessorsJSON, indAccInd);
    //intVec *indicesVec = MODEL_getIndices(model, accessorJSON);
    MODEL_getTextures(model);

#if 0
    std::vector<GLuint> Model::getIndices(json accessor)
    {
        std::vector<GLuint> indices;

        // Get properties from the accessor
        unsigned int buffViewInd = accessor.value("bufferView", 0);
        unsigned int count = accessor["count"];
        unsigned int accByteOffset = accessor.value("byteOffset", 0);
        unsigned int componentType = accessor["componentType"];

        // Get properties from the bufferView
        json bufferView = JSON["bufferViews"][buffViewInd];
        unsigned int byteOffset = bufferView["byteOffset"];

        // Get indices with regards to their type: unsigned int, unsigned short, or short
        unsigned int beginningOfData = byteOffset + accByteOffset;
        if (componentType == 5125)
        {
            for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i)
            {
                unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
                unsigned int value;
                std::memcpy(&value, bytes, sizeof(unsigned int));
                indices.push_back((GLuint)value);
            }
        }
        else if (componentType == 5123)
        {
            for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
            {
                unsigned char bytes[] = { data[i++], data[i++] };
                unsigned short value;
                std::memcpy(&value, bytes, sizeof(unsigned short));
                indices.push_back((GLuint)value);
            }
        }
        else if (componentType == 5122)
        {
            for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
            {
                unsigned char bytes[] = { data[i++], data[i++] };
                short value;
                std::memcpy(&value, bytes, sizeof(short));
                indices.push_back((GLuint)value);
            }
        }

        return indices;
    }
#endif





#if 0
    void Model::loadMesh(unsigned int indMesh)
    {
        // Get all accessor indices
        unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
        unsigned int normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
        unsigned int texAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
        unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];

        // Use accessor indices to get all vertices components
        std::vector<float> posVec = getFloats(JSON["accessors"][posAccInd]);
        std::vector<glm::vec3> positions = groupFloatsVec3(posVec);
        std::vector<float> normalVec = getFloats(JSON["accessors"][normalAccInd]);
        std::vector<glm::vec3> normals = groupFloatsVec3(normalVec);
        std::vector<float> texVec = getFloats(JSON["accessors"][texAccInd]);
        std::vector<glm::vec2> texUVs = groupFloatsVec2(texVec);

        // Combine all the vertex components and also get the indices and textures
        std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
        std::vector<GLuint> indices = getIndices(JSON["accessors"][indAccInd]);
        std::vector<Texture> textures = getTextures();

        // Combine the vertices, indices, and textures into a mesh
        meshes.push_back(Mesh(vertices, indices, textures));
    }
#endif


    return 0;
}

int MODEL_traverseNode(Model *model, int nextNode, mat4 matrix) {

#if 0
    printf("Data: \n");
    for (int i = 0; i < model -> dataLength; i++) {
      printf("%x", *(model -> data + i));
    }
    printf("\n");

    printf("Length of data: %d\n", model -> dataLength);
#endif

    char *string = NULL;
    cJSON *element = NULL;

    //string = cJSON_Print(model -> modelJSON);
    //printf("%s\n", string);


    int status = 1;
    if (model -> modelJSON == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        status = 0;
        return -1;
    }

    cJSON *nodes = cJSON_GetObjectItemCaseSensitive(model -> modelJSON, "nodes");
    cJSON *node = cJSON_GetArrayItem(nodes, nextNode);

#if 0
    string = cJSON_Print(node);
    if (string == NULL) {
        fprintf(stderr, "Failed to print nodes.\n");
    }
    else {
        printf("Node: %s\n", string);
    }
#endif

    // Get translation if it exists
    vec3 translation;
    glm_vec3_one(translation);
    cJSON *transitionJSON = cJSON_GetObjectItemCaseSensitive(node, "translation");
    int i, j = 0;
    cJSON_ArrayForEach(element, transitionJSON) {
        translation[i] = element->valuedouble;
        i++;
    }
    printf("%f %f %f\n", translation[0], translation[1], translation[2]);

    versor rotation;
    glm_quat_identity(rotation);
    cJSON *rotationJSON = cJSON_GetObjectItemCaseSensitive(node, "rotation");
    if (rotationJSON != NULL) {
        i = 0;
        cJSON_ArrayForEach(element, rotationJSON) {
            switch (i) {
                case 0:
                    rotation[3] = element->valuedouble;
                    break;

                case 1:
                    rotation[0] = element->valuedouble;
                    break;

                case 2:
                    rotation[1] = element->valuedouble;
                    break;

                case 3:
                    rotation[2] = element->valuedouble;
                    break;
            }
            i++;
        }
    }
    else {
        printf("Did not find rotation.\n");
    }
    //printf("%f %f %f %f\n", rotation[0], rotation[1], rotation[2], rotation[3]);

    vec3 scale;
    glm_vec3_one(scale);
    cJSON *scaleJSON = cJSON_GetObjectItemCaseSensitive(node, "scale");
    if (scaleJSON != NULL) {
        i = 0;
        cJSON_ArrayForEach(element, scaleJSON) {
            scale[i] = element->valuedouble;
            i++;
        }
    }
    else {
        printf("Did not find scale.\n");
    }
    //printf("%f %f %f\n", scale[0], scale[1], scale[2]);

    mat4 matNode  = GLM_MAT4_IDENTITY_INIT;
    cJSON *matNodeJSON = cJSON_GetObjectItemCaseSensitive(node, "matrix");
    if (matNodeJSON != NULL) {
        i = j = 0;
        cJSON_ArrayForEach(element, matNodeJSON) {
            matNode[i][j] = element->valuedouble;
            j++;
            if (j == 4) {
                j = 0;
                i++;
            }
        }
    }
    else {
        printf("Did not find matrix.\n");
    }
    //printf("%f %f %f %f\n", matNode[0], matNode[1], matNode[2], matNode[3]);

    mat4 trans = GLM_MAT4_IDENTITY_INIT;
    mat4 rot = GLM_MAT4_IDENTITY_INIT;
    mat4 sca = GLM_MAT4_IDENTITY_INIT;

    glm_translate(trans, translation);
    glm_quat_mat4(rotation, rot);
    glm_scale(sca, scale);

    mat4 matNextNode = GLM_MAT4_IDENTITY_INIT;
    glm_mat4_mul(rot, sca, matNextNode);
    glm_mat4_mul(trans, matNextNode, matNextNode);
    glm_mat4_mul(matNode, matNextNode, matNextNode);
    glm_mat4_mul(matrix, matNextNode, matNextNode);

    // if mesh in node, save all transformation matrices
    cJSON *meshNodeJSON = cJSON_GetObjectItemCaseSensitive(node, "mesh");
    if (meshNodeJSON != NULL) {
        glm_vec3_copy(translation, *(model -> translation_mesh_array + nextNode));
        glm_vec4_copy(rotation, *(model -> rotation_mesh_array + nextNode));
        glm_vec3_copy(scale, *(model -> scale_mesh_array + nextNode));
        glm_mat4_copy(matNextNode, *(model -> matrix_mesh_array + nextNode));
    }

    //printf("%f %f %f\n", *(*(model -> translation_mesh_array + nextNode) + 0), *(*(model -> translation_mesh_array + nextNode) + 1), *(*(model -> translation_mesh_array + nextNode) + 2));

    //loadMesh(node["mesh"]);
    unsigned int indMesh = meshNodeJSON -> valueint;
    MODEL_loadMesh(model, indMesh);

    // TODO Traverse children nodesd

    // free memory
    cJSON_Delete(element);
    free(string);
    return 0;
}


int MODEL_delete(Model *model) {
    cJSON_Delete(model -> modelJSON);
    free(model -> data);

    // free the transformation arrays
    free(model -> translation_mesh_array);
    free(model -> rotation_mesh_array);
    free(model -> scale_mesh_array);
    free(model -> matrix_mesh_array);

    // TODO: free texture memory

    free(model);
    return 0;
}
