#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffman.h"

int main()
{
    Node *tree;
    char input[] = "hello world";

    // start measurement
    char *compressed = encode(input, &tree);
    // stop measurement

    char *decompressed = decode(compressed, tree);

    // compare decompressed with original
    printf("original text: %s\n", input);
    printf("decompressed:  %s\n", decompressed);
    assert(!strcmp(input, decompressed));
    
    free(compressed);
    free(decompressed);

    return 0;
}
