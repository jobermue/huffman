/**
 * @brief Interface for Huffman encoding/decoding.
 */

#ifndef HUFFMAN_H
#define HUFFMAN_H

#define MAX_STRING_LENGTH (4096)  //max length of a string (including null termination)

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
 * @param input Text to be compressed
 * @param tree  The Huffman tree constructed from the input text is returned here
 */
struct bytestream encode(const char *input, Node **tree);


/**
 * @brief Decodes the provided input 
 *
 * @param input Text to be decompressed
 * @param tree  The Huffman tree that was constructed from the original text
 */
char *decode(const struct bytestream input, const Node *tree);

 
#endif
