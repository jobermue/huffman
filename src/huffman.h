/**
 * @brief Interface for Huffman encoding/decoding.
 */

#ifndef HUFFMAN_H
#define HUFFMAN_H

#define NR_OF_ASCII_CHARS (128)
#define NR_OF_CHARS (NR_OF_ASCII_CHARS)
#define NR_OF_NODES (NR_OF_CHARS-1) // exclude '\0'

/* Node of the huffman tree */
struct node{
    int value;
    char letter;
    struct node *left,*right;
};

typedef struct node Node;

struct code {
    int code;
    unsigned int len;
};

struct bytestream {
    unsigned char *stream;
    int len; // length in bits
};


/**
 * @brief Huffman encodes the provided input and generates a Huffman tree.
 *
 * @param input          Text to be compressed
 * @param tree           The Huffman tree constructed from the input text is returned here
 * @param pool_of_nodes  Data structure with enough space for Huffman tree (2n-1)
 */
struct bytestream encode(const char *input, Node **tree, Node *pool_of_nodes);

/**
 * @brief Decodes the provided input 
 *
 * @param input Text to be decompressed
 * @param tree  The Huffman tree that was constructed from the original text
 */
char *decode(const struct bytestream input, const Node *tree);

 
#endif
