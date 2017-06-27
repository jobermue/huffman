/****************************************************************************/
/*                A simple implementation of the Huffman coding             */
/*                author: danielscocco@gmail.com                            */
/*                                                                          */
/****************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "stack.h"
#include "huffman.h"


#define NR_OF_CHARS (UCHAR_MAX)
#define NR_OF_NODES (NR_OF_CHARS)

#ifdef ENDEBUG
#define DEBUG(...) do { fprintf(stderr, __VA_ARGS__); } while(0)
#else
#define DEBUG(...)
#endif

/**
 * @brief Finds and returns the smallest sub-tree in the forest that is different from differentFrom
 *
 * @param array          Array of subtrees
 * @param nr_of_nodes    Max number of nodes in forrest
 * @param differentFrom  Index of a subtree in the array
 */
static int findSmallest (Node *array[], int nr_of_nodes,  int differentFrom)
{
    int i, smallest;

    /* Initialize 'smallest' with a valid node (value != -1) */
    i = 0;
    while (array[i]->value==-1)
        i++;
    smallest = i;
    if (i == differentFrom){
        i++;
        while (array[i]->value == -1)
            i++;
        smallest = i;
    }

    /* Find smallest node (apart from differentFrom) */
    for (i=1; i < nr_of_nodes; i++){
        if (array[i]->value == -1)
            continue;
        if (i == differentFrom)
            continue;
        if (array[i]->value < array[smallest]->value)
            smallest = i;
    }

    return smallest;
}

/**
 * @brief Builds the Huffman tree and returns its address by reference
 *
 * @param tree        The resulting Huffman tree
 * @param input_text  The text the Huffman tree will be created for
 */
static void buildHuffmanTree (Node **tree, const char *input_text)
{
    Node *temp;
    Node **array;
    int subTrees;
    int smallOne,smallTwo;
    int letter_frequencies[NR_OF_CHARS];

    // initialize forest with single node trees (one per character)
    /* for (i=0; i<NR_OF_NODES; i++){ */
    /*     array[i] = malloc(sizeof(Node)); */
    /*     array[i]->value = englishLetterFrequencies[i]; */
    /*     array[i]->letter = i; */
    /*     array[i]->left = NULL; */
    /*     array[i]->right = NULL; */
    /* } */

    /* Get letter frequencies in input text */
    for (int i = 0; i < NR_OF_CHARS; i++) {
        letter_frequencies[i] = 0;
    }
    for (int i = 0; i < strlen(input_text); i++) {
        letter_frequencies[(unsigned char)input_text[i]]++;
    }

    /* Initialize forest with single node trees (one per character) */
    int nr_of_nodes = 0;
    for (int i = 0; i < NR_OF_CHARS; i++) {
        if (letter_frequencies[i] > 0) {
            DEBUG("letter frequency of %c: %i\n", (char) i, letter_frequencies[i]);
            nr_of_nodes++;
        }
    }
    array = malloc(sizeof(Node*) * nr_of_nodes);
    int j = 0;
    for (int i = 0; i < NR_OF_CHARS; i++) {
        if (letter_frequencies[i] > 0) {
            assert (j < nr_of_nodes);
            array[j] = malloc(sizeof(Node));
            array[j]->value = letter_frequencies[i];
            array[j]->letter = (char) i;
            array[j]->left = NULL;
            array[j]->right = NULL;
            j++;
        }
    }

    /* Combine subtrees into a single tree */
    subTrees = nr_of_nodes;
    while (subTrees>1){
        smallOne = findSmallest(array, nr_of_nodes, -1);
        smallTwo = findSmallest(array, nr_of_nodes, smallOne);
        temp = array[smallOne];
        array[smallOne] = malloc(sizeof(Node));
        array[smallOne]->value  = temp->value + array[smallTwo]->value;
        array[smallOne]->letter = 0;
        array[smallOne]->left   = array[smallTwo];
        array[smallOne]->right  = temp;
        array[smallTwo]->value  = -1; //to 'remove' node from forest
        subTrees--;
    }

    *tree = array[smallOne];
}

/**
 * @brief Builds the table with the bits for each letter. 
 *
 * @param codeTable              The resulting code table
 * @param tree          The Huffman tree used to generate the code table
 * @param nr_of_leaves  The maximum number of leaves in the Huffman tree      
 */
static struct code *fillTable(struct code *codeTable, const Node *tree, int nr_of_leaves)
{
    int code = 0;
    int len = 0;
    Node *node;
    struct stack *s = NULL;
    struct stack_entry *e;

    /* note: nr of inner nodes in a tree of n leaves: n-1
     * max size of stack: n
     */
    s = stack_init(nr_of_leaves);
    stack_push(s, &(struct stack_entry) {tree,0,0});
    while (stack_size(s) > 0) {
        e = stack_pop(s);
        node = e->node;
        code = e->code;
        len = e->len;
        if ((node->left == NULL) && (node->right == NULL)) { // if node is a leaf
            codeTable[(int)node->letter] = (struct code) {code, len};
        } else {
            stack_push(s, &(struct stack_entry) {node->right, (code<<1)|1, len+1});
            stack_push(s, &(struct stack_entry) {node->left,   code<<1,    len+1});
        }
    }
    return codeTable;
}

/**
 * @brief Invert codes (so they can be used with mod operator by compressFile function)
 *
 * @param codeTable    original code table created from Huffman tree
 * @param invCodeTable code table with inverted code words
 */
static void invertCodes(struct code codeTable[], struct code invCodeTable[])
{
    int i, n, copy;

    for (i=0; i<NR_OF_CHARS; i++){
        n = codeTable[i].code;
        if (n != -1) {
            copy = 0;
            for (int j = 0; j<codeTable[i].len; j++) {
                copy = (copy<<1) | (n & 0x01);
                n = n>>1;
            }
            invCodeTable[i] = (struct code) {.code=copy, .len = codeTable->len};
        }
    }
}


/**
 * @brief function to compress the input
 *
 * @param input Text to be compressed
 * @param codeTable inverted Huffman code table
*/
static struct bytestream compress(const char *input, struct code codeTable[], struct code invCodeTable[])
{
    char bit, c, x = 0;
    int n,length,bitsLeft = 8;
    int originalBits = 0, compressedBits = 0;
    int i = 0, compressedBytes = 0;
    unsigned char *output = { 0 };

    for (i = 0, c = input[i]; c != 0; c = input[++i]) {
        compressedBits += codeTable[(unsigned char)c].len;
    }
    compressedBytes = (compressedBits + 8 - 1)/8; // ceil(compressedBits/8)
    compressedBytes += 1;     // increase memory by one byte (for null termination)
    int allocatedBytes = compressedBytes;

    // allocate memory
    output = malloc(sizeof(char) * compressedBytes);

    i = 0;
    bitsLeft = 8;
    originalBits = 0;
    compressedBytes = 0;
    compressedBits = 0;

    c = input[i];
    while (c != 0)
    {
        originalBits++;
        length = codeTable[(unsigned char)c].len;
        n = invCodeTable[(unsigned char)c].code;
 
        while (length>0)
        {
            compressedBits++;
            bit = (n & 0x01);
            n = n>>1;
            x = x | bit;
            bitsLeft--;
            length--;
            if (bitsLeft==0)
            {
                //fputc(x,output);
                compressedBytes++;
                output[compressedBytes - 1] = x;

                x = 0;
                bitsLeft = 8;
            }
            x = x << 1;
        }
        i++;
        c = input[i];
    }

    if (bitsLeft!=8)
    {
        x = x << (bitsLeft-1);
        // fputc(x,output);

        compressedBytes++;
        output[compressedBytes - 1] = x;
    }
    output[compressedBytes] = 0; // null terminate string

    assert(allocatedBytes == compressedBytes+1);

    /*print details of compression on the screen*/
    DEBUG("Original bits = %d\n",originalBits*8);
    DEBUG("Compressed bits = %d\n",compressedBits);
    DEBUG("Saved %.2f%% of memory\n",((float)compressedBits/(originalBits*8))*100);

    struct bytestream result = {.stream = output, .len = compressedBits};
    return result;
}

struct bytestream encode(const char *input, Node **tree)
{
    struct code codeTable[NR_OF_CHARS], invCodeTable[NR_OF_CHARS];

    for (int i = 0; i < NR_OF_CHARS; i++) {
        codeTable[i] = (struct code) {-1, 0};
        invCodeTable[i] = (struct code) {-1, 0};
    }

    buildHuffmanTree(tree, input);
    fillTable(codeTable, *tree, NR_OF_CHARS);

    invertCodes(codeTable, invCodeTable);

    return compress(input, codeTable, invCodeTable);
}


/**
 * @brief Decodes the provided input 
 *
 * @param input Text to be decompressed
 * @param tree  The Huffman tree that was constructed from the original text
 */
char *decode(const struct bytestream input, const Node *tree)
{
    const Node *current = tree;
    char c,bit;
    unsigned char mask = 1 << 7;
    int i = 0, decompressedBytes = 0;
    char *output = { 0 };

    for (int j = 0; j*8 < input.len; j++) {
        c = input.stream[j];
        for (i=0;i<8 && j*8+i<input.len;i++){
            bit = c & mask;
            c = c << 1;
            if (bit==0)
                current = current->left;
            else
                current = current->right;

            if((current->left != NULL) || (current->right != NULL))
                continue;

            char current_c = current->letter;

            decompressedBytes++;
            output = realloc(output, decompressedBytes);
            output[decompressedBytes - 1] = current_c;

            current = tree;

        }
    }
    output = realloc(output, decompressedBytes + 1);
    output[decompressedBytes] = 0;

    return output;
}
