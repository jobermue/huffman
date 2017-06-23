/**
 * @brief Interface for Huffman encoding/decoding.
 */

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdint.h>

/* Node of the huffman tree */
struct node{
    int value;
    char letter;
    struct node *left,*right;
};

typedef struct node Node;

typedef struct
{
    char *val;
    uint16_t *cnt;
    uint16_t val_cnt;
} dict_t;

/**
 * @brief Huffman encodes the provided input and generates a Huffman tree.
 *
 * @param input Text to be compressed
 * @param tree  The Huffman tree constructed from the input text is returned here
 */
char *encode(const char *input, Node **tree);


/**
 * @brief Decodes the provided input 
 *
 * @param input Text to be decompressed
 * @param tree  The Huffman tree that was constructed from the original text
 */
char *decode(const char *input, const Node *tree);

 
#endif
