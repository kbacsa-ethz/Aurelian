//
// Created by sbeuret on 14.05.22.
//

// free the positions array
void POSITIONS_ARRAY_free(PositionsArray* positions_array_ptr){
    free(positions_array_ptr->positions_ptr);
    positions_array_ptr->positions_ptr = NULL;
    positions_array_ptr->size_positions = 0;
}

// free the normals array
void NORMALS_ARRAY_free(NormalsArray* normals_array_ptr){
    free(normals_array_ptr->normals_ptr);
    normals_array_ptr->normals_ptr = NULL;
    normals_array_ptr->size_normals = 0;
}

// free the texture UVs array
void TEXTUVS_ARRAY_free(TextUVsArray* textUVs_array_ptr){
    free(textUVs_array_ptr->textUVs_ptr);
    textUVs_array_ptr->textUVs_ptr = NULL;
    textUVs_array_ptr->size_textUVs = 0;
}

// free the indices array
void INDICES_ARRAY_free(IndicesArray* indices_array_ptr){
    free(indices_array_ptr->indices_ptr);
    indices_array_ptr->indices_ptr = NULL;
    indices_array_ptr->size_indices = 0;
}
