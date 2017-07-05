/**
 * @brief Interface for Huffman encoding/decoding.
 */

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdint.h>

#define MAX_STRING_LENGTH (4096)  //max length of a string (including null termination)

#define NR_OF_ASCII_CHARS (128)
#define NR_OF_CHARS (NR_OF_ASCII_CHARS)
#define NR_OF_NODES (NR_OF_CHARS) // includes '\0'
#define MIN_VALID_CHAR (0)

/* Node of the huffman tree */
struct node{
    uint16_t value;
    char letter;
    struct node *left,*right;
};

typedef struct node Node;

typedef uint16_t codeword_t;

struct code {
    codeword_t codeword;
    uint8_t    len;
};

struct bytestream {
    char *data;
    uint16_t len; // length in bits
};


/**
 * @brief Huffman encodes the provided input and generates a Huffman tree.
 *
 * @param input          Text to be compressed
 * @param tree           The Huffman tree constructed from the input text is returned here
 * @param pool_of_nodes  Data structure with enough space for Huffman tree (2n-1)
 * @param output         Data structure with enough space for encoded output 7/8*MAX_STRING_LENGTH
 */
struct bytestream encode(const char *input, Node **tree, Node *pool_of_nodes, char *output);


/**
 * @brief Decodes the provided input 
 *
 * @param input Text to be decompressed
 * @param tree  The Huffman tree that was constructed from the original text
 */
char *decode(const struct bytestream input, const Node *tree);

 
#endif
