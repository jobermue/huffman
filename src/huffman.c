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
#include "queue.h"
#include "huffman.h"
#include "insertion_sort.h"
#include "merge_sort.h"

#define NR_OF_ASCII_CHARS (128)
#define NR_OF_CHARS (NR_OF_ASCII_CHARS)
#define NR_OF_NODES (NR_OF_CHARS) // includes '\0'
#define MIN_VALID_CHAR (0)

#ifdef ENDEBUG
#define DEBUG(...) do { fprintf(stderr, __VA_ARGS__); } while(0)
#else
#define DEBUG(...)
#endif

#ifdef ENDEBUG
/**
 * @brief convert code to printable string (binary)
 */
static void print_code(const struct code *code)
{
    int n = code->code;
    for (int j = 0; j<code->len; j++) {
        if (n & 1)
            DEBUG("1");
        else
            DEBUG("0");

        n >>= 1;
    }
    fprintf(stderr, " [len: %i]", code->len);
}
#endif

static Node *get_min(queue_t *q1, queue_t *q2)
{
    Node *ret = NULL;

    ret = queue_empty(q2) ? queue_dequeue(q1) : ret;
    ret = !queue_empty(q2) && (queue_empty(q1) || queue_peek(q2) < queue_peek(q1)) ? queue_dequeue(q2) : ret;
    ret = !queue_empty(q2) && !(queue_empty(q1) || queue_peek(q2) < queue_peek(q1)) ? queue_dequeue(q1) : ret;

    return ret;
}


/**
 * @brief Builds the Huffman tree and returns its address by reference
 *
 * @param pool_of_nodes   Data structure with enough space for Huffman tree (2n-1)
 * @param input_text      The text the Huffman tree will be created for 
 *                        (filled up with 0's to MAX_STRING_LENGTH)
 * @return root node of the resulting Huffman tree
 */
/* ai: instruction "buildHuffmanTree" is entered with @nr_of_chars = 128;  */
/* ai: instruction "buildHuffmanTree" is entered with @strlen = 4096;  */
static Node *buildHuffmanTree (Node *pool_of_nodes, const char *input_text)
{
    Node *combined;
    Node *array[NR_OF_NODES];
    Node *array_q2[NR_OF_NODES-1];
    queue_t q1, q2;
    int subTrees;
    Node *smallOne, *smallTwo;
    int letter_frequencies[NR_OF_CHARS];

    /* Get letter frequencies in input text */
    #pragma loopbound min 0 max 128
    /* ai?: loop here max @nr_of_chars; */
    for (int i = 0; i < NR_OF_CHARS; i++) {
        letter_frequencies[i] = 0;
    }
    //assert(strlen(input_text) < 4096);
    #pragma loopbound min 0 max 4096
    /* ai?: loop here max @strlen ; */
    for (int i = 0; i < MAX_STRING_LENGTH; i++) {
        letter_frequencies[(unsigned char)input_text[i]]++;
    }

    /* Initialize forest with single node trees (one per character) */
#ifdef ENDEBUG
    #pragma loopbound min 0 max 128
    /* ai?: loop here max @nr_of_chars; */
    for (int i = MIN_VALID_CHAR; i < NR_OF_CHARS; i++) {
        if (letter_frequencies[i] > 0) {
            DEBUG("letter frequency of %c: %i\n", (char) i, letter_frequencies[i]);
        }
    }
#endif
    int j = 0;
    #pragma loopbound min 0 max 128
    /* ai?: loop here loops max @nr_of_chars; */
    for (int i = MIN_VALID_CHAR; i < NR_OF_CHARS; i++) {
        //assert (j < NR_OF_NODES);
        array[j] = &pool_of_nodes[j];
        array[j]->value = letter_frequencies[i];
        array[j]->letter = (char) i;
        array[j]->left = NULL;
        array[j]->right = NULL;
        j++;
    }

    /* Sort forest */
    //insertion_sort(array, nr_of_nodes);
    merge_sort(array, nr_of_nodes);

    /* Combine subtrees into a single tree */
    queue_init(&q1, array, NR_OF_NODES, 0, NR_OF_NODES-1);
    queue_init(&q2, array_q2, NR_OF_NODES-1, 0, -1);
    #pragma loopbound min 0 max 127
    /* ai?: loop here loops max @nr_of_chars-1; */
    for (subTrees = NR_OF_NODES; subTrees>1; subTrees--) {
        smallOne = get_min(&q1, &q2);
        smallTwo = get_min(&q1, &q2);
        combined = &pool_of_nodes[j++];
        combined->value  = smallOne->value + smallTwo->value;
        combined->letter = 0;
        combined->left   = smallTwo;
        combined->right  = smallOne;
        queue_enqueue(&q2, combined);
    }

    return queue_dequeue(&q2);
}

/**
 * @brief Builds the table with the bits for each letter. 
 *
 * @param codeTable              The resulting code table
 * @param tree          The Huffman tree used to generate the code table
 * @param nr_of_leaves  The maximum number of leaves in the Huffman tree      
 */
/* ai?: instruction fillTable is entered with @nr_of_leaves <= NR_OF_CHARS (include '\0');  */
/* ai: instruction "fillTable" is entered with @nr_of_leaves = 128;  */
static struct code *fillTable(struct code *codeTable, const Node *tree, int nr_of_leaves)
{
    int code = 0;
    int len = 0;
    const Node *node;
    struct stack s;
    struct stack_entry entries[NR_OF_NODES];
    struct stack_entry *e;

#ifndef HOST_COMPILE
    __llvm_pcmarker(3);
#endif

    /* note: nr of inner nodes in a tree of n leaves: n-1
     * max size of stack: n
     * nr of total nodes (inner nodes + leaves) = 2n-1
     */
    stack_init(&s, entries, nr_of_leaves);
    stack_push(&s, &(struct stack_entry) {tree,0,0});
    #pragma loopbound min 0 max 255
    /* ai: loop here max 2*@nr_of_leaves-1; */
    for (int i = 0; i < 2*NR_OF_NODES-1; i++) {
        e = stack_pop(&s);
        node = e->node;
        code = e->code;
        len = e->len;
        if ((node->left == NULL) && (node->right == NULL)) { // if node is a leaf
            /* ai: label here = "fillTable_if"; */
#ifndef HOST_COMPILE
            __llvm_pcmarker(4);
#endif
            codeTable[(int)node->letter] = (struct code) {code, len};
        } else {
            stack_push(&s, &(struct stack_entry) {node->right, (code<<1)|1, len+1});
            stack_push(&s, &(struct stack_entry) {node->left,   code<<1,    len+1});
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
/* ai: instruction "invertCodes" is entered with @nr_of_chars = 128;  */
/* ai: instruction "invertCodes" is entered with @max_code_length = 15;  */
static void invertCodes(struct code codeTable[], struct code invCodeTable[])
{
    int i, n, copy;

    #pragma loopbound min 0 max 128
    /* ai?: loop here max @nr_of_chars; */
    for (i=0; i<NR_OF_CHARS; i++){
        n = codeTable[i].code;
        copy = 0;
        /* max length of a code = max height of Huffman tree <= n */
        /* can be further bounded - see https://groups.google.com/forum/#!topic/comp.compression/m5pj1lDoeU8 */
        #pragma loopbound min 0 max 15
        /* ai?: loop here max @max_code_length; */
        for (int j = 0; j<codeTable[i].len; j++) {
            /* max length of code only for at most 2 codes possible, others are shorter 
             * (or if all have equal length, max possible length is not reached) */
            /* flow is (nr_of_chars+2)*(nr_of_chars-1)/2  - TODO: update this */
            /* ai: label here = "invertCodes_inner"; */
            copy = (copy<<1) | (n & 0x01);
            n = n>>1;
        }
        invCodeTable[i] = (struct code) {.code=copy, .len = codeTable[i].len};
    }
}


/**
 * @brief function to compress the input
 *
 * @param input Text to be compressed
 * @param codeTable inverted Huffman code table
*/
/* ai: instruction "compress" is entered with @strlen = 4096;  */
static struct bytestream compress(const char *input, struct code codeTable[], struct code invCodeTable[])
{
    char bit, c, x = 0;
    int n,length,bitsLeft = 8;
    int originalBits = 0, compressedBits = 0;
    int compressedBytes = 0;
    unsigned char output[(7*MAX_STRING_LENGTH)/8];

#ifndef HOST_COMPILE
    __llvm_pcmarker(5);
#endif

    originalBits = 0; //TODO: rename to originalBytes, as it actually counts bytes

    #pragma loopbound min 0 max 4096
    /* ai?: loop here max @strlen; */
    for (int i = 0; i < MAX_STRING_LENGTH; i++) {
        c = input[i];
        originalBits++;
        length = codeTable[(unsigned char)c].len;
        n = invCodeTable[(unsigned char)c].code;
 
        #pragma loopbound min 0 max 15
        /* ai?: loop here max 15; */
        for (int j=0; j < length; j++) {
            /* ai: label here = "compress_inner_while"; */
#ifndef HOST_COMPILE
            __llvm_pcmarker(6);
#endif
            compressedBits++;
            bit = (n & 0x01);
            n = n>>1;
            x = x | bit;
            bitsLeft--;
            if (bitsLeft==0) {
              /* ai: label here = "compress_inner_if"; */
#ifndef HOST_COMPILE
               __llvm_pcmarker(7);
#endif
                compressedBytes++;
                output[compressedBytes - 1] = x;

                x = 0;
                bitsLeft = 8;
            }
            x = x << 1;
        }
    }

    x = (bitsLeft!=8) ? x << (bitsLeft-1) : x;
    compressedBytes = (bitsLeft!=8) ? compressedBytes+1 : compressedBytes;
    output[compressedBytes - 1] = (bitsLeft!=8) ? x : output[compressedBytes - 1];
    //assert(allocatedBytes == compressedBytes+1);

    /*print details of compression on the screen*/
    DEBUG("Original bits = %d\n",originalBits*8);
    DEBUG("Compressed bits = %d\n",compressedBits);
    DEBUG("Compressed to %.2f%% of memory\n",((float)compressedBits/(originalBits*8))*100);

    struct bytestream result = {.stream = output, .len = compressedBits};
    return result;
}

struct bytestream encode(const char *input, Node **tree)
{
    struct code codeTable[NR_OF_CHARS], invCodeTable[NR_OF_CHARS];
    Node pool_of_nodes[2*NR_OF_CHARS-1];

    #pragma loopbound min 0 max 256
    for (int i = 0; i < NR_OF_CHARS; i++) {
        codeTable[i] = (struct code) {-1, 0};
        invCodeTable[i] = (struct code) {-1, 0};
    }

    *tree = buildHuffmanTree(pool_of_nodes, input);
    fillTable(codeTable, *tree, NR_OF_CHARS);
    invertCodes(codeTable, invCodeTable);

    /* print code table */
#ifdef ENDEBUG
    for (int i = 0; i < NR_OF_CHARS; i++) {
        if (invCodeTable[i].len != 0) {
            DEBUG("codeTable[%i] - %c: ", i, (char) i);
            print_code(&invCodeTable[i]);
            DEBUG("\n");
        }
    }
#endif

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
