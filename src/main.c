#include <stdio.h>
#include <stdlib.h>

// Debuggig macros
#define PRINT_VAR(X) printf(#X " is %d at address %p\n", X, &X);
#define SWAP(A, B) A ^= B ^= A ^= B;

int main(int argc, char *argv[])
{
    printf("Hello world.");
    return 0;
}
