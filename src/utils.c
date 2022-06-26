#include <stdio.h>
#include <stdlib.h>


char *UTILS_readfile(const char *filename) {

    // from https://stackoverflow.com/questions/3747086/reading-the-whole-text-file-into-a-char-array-in-c
    char *shaderpointer;
    long length;
    FILE *f = fopen (filename, "rb");

    if (f) {
      fseek (f, 0, SEEK_END);
      length = ftell (f);
      fseek (f, 0, SEEK_SET);
      shaderpointer = (char*)malloc ((length+1)*sizeof(char));
      if (shaderpointer) {
        fread (shaderpointer, sizeof(char), length, f);
        *(shaderpointer+length) = '\0'; // Prevent garbage at the end of string.
      }

      fclose (f);
    }

    return shaderpointer;

}
