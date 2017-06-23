#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "huffman.h"

int main()
{
    Node *tree;
    char input[] = "hello world";
    int codeTable[27], invCodeTable[27];

    // start measurement
    /* buildHuffmanTree(&tree); */
    /* fillTable(codeTable, tree, 0); */
    /* invertCodes(codeTable,invCodeTable); */
    /* char *compressed = compress(input, invCodeTable); */
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
