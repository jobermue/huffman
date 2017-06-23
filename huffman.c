/****************************************************************************/
/*                A simple implementation of the Huffman coding             */
/*                author: danielscocco@gmail.com                            */
/*                                                                          */
/****************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "huffman.h"

/* static unsigned int log10(unsigned int v) */
/* { */
/*     return (v >= 1000000000u) ? 9 : (v >= 100000000u) ? 8 :  */
/*         (v >= 10000000u) ? 7 : (v >= 1000000u) ? 6 :  */
/*         (v >= 100000u) ? 5 : (v >= 10000u) ? 4 : */
/*         (v >= 1000u) ? 3 : (v >= 100u) ? 2 : (v >= 10u) ? 1u : 0u;  */
/* } */

#define len(x) (log10(x)+1)



/* 81 = 8.1%, 128 = 12.8% and so on. The 27th frequency is the space. Source is Wikipedia */
int englishLetterFrequencies [27] = {81, 15, 28, 43, 128, 23, 20, 61, 71, 2, 1, 40, 24, 69, 76, 20, 1, 61, 64, 91, 28, 10, 24, 1, 20, 1, 130};

/**
 * @brief Finds and returns the smallest sub-tree in the forest that is different from differentFrom
 *
 * @param array Array of subtrees
 * @param differentFrom Index of a subtree in the array
 */
static int findSmallest (Node *array[], int differentFrom)
{
    int smallest;
    int i = 0;

    // initialize 'smallest' with a valid node (value != -1)
    while (array[i]->value==-1)
        i++;
    smallest = i;
    if (i == differentFrom){
        i++;
        while (array[i]->value == -1)
            i++;
        smallest = i;
    }

    // find smallest node (apart from differentFrom)
    for (i=1; i<27; i++){
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
 * @param tree The resulting Huffman tree
 */
static void buildHuffmanTree (Node **tree)
{
    Node *temp;
    Node *array[27]; //TODO: replace 27 by a constant; and change to 255
    int i, subTrees = 27;
    int smallOne,smallTwo;

    // initialize forest with single node trees (one per character)
    for (i=0; i<27; i++){
        array[i] = malloc(sizeof(Node));
        array[i]->value = englishLetterFrequencies[i];
        array[i]->letter = i;
        array[i]->left = NULL;
        array[i]->right = NULL;
    }

    // combine subtrees into a single tree
    while (subTrees>1){
        smallOne = findSmallest(array, -1);
        smallTwo = findSmallest(array, smallOne);
        temp = array[smallOne];
        array[smallOne] = malloc(sizeof(Node));
        array[smallOne]->value  = temp->value + array[smallTwo]->value;
        array[smallOne]->letter = 127; //TODO: why 127??
        array[smallOne]->left   = array[smallTwo];
        array[smallOne]->right  = temp;
        array[smallTwo]->value  = -1; //to 'remove' node from forrest
        subTrees--;
    }

    *tree = array[smallOne];
}

/**
 * @brief Builds the table with the bits for each letter. 
 *        1 stands for binary 0 and 2 for binary 1 (used to facilitate arithmetic)
 *
 * @param codeTable
 * @param tree
 * @param code
 */
static void fillTable(unsigned int codeTable[], Node *tree, int code)
{
    if (tree->letter < 27) { // if node is a leaf
        codeTable[(int)tree->letter] = code;
    } else {
        fillTable(codeTable, tree->left, code*10+1);
        fillTable(codeTable, tree->right, code*10+2);
    }
}

/**
 * @brief Invert codes (so they can be used with mod operator by compressFile function)
 *
 * @param codeTable    original code table created from Huffman tree
 * @param invCodeTable code table with inverted code words
 */
static void invertCodes(unsigned int codeTable[], unsigned int invCodeTable[])
{
    int i, n, copy;

    for (i=0; i<27; i++){
        n = codeTable[i];
        copy = 0;
        while (n > 0){
            copy = copy * 10 + n %10;
            n /= 10;
        }
        invCodeTable[i] = copy;
    }
}


/**
 * @brief function to compress the input
 *
 * @param input Text to be compressed
 * @param codeTable Huffman code table
*/
static char* compress(char const *input, unsigned int codeTable[])
{
    char bit, c, x = 0;
    int n,length,bitsLeft = 8;
    int originalBits = 0, compressedBits = 0;
    int i = 0, compressedBytes = 0;
    char *output = { 0 };

    c = input[i];
    while (c != 0)
    {
        originalBits++;
        if (c==32){
            length = len(codeTable[26]);
            n = codeTable[26];
        }
        else{
            length=len(codeTable[c-97]);
            n = codeTable[c-97];
        }

        while (length>0)
        {
            compressedBits++;
            bit = n % 10 - 1;
            n /= 10;
            x = x | bit;
            bitsLeft--;
            length--;
            if (bitsLeft==0)
            {
                //fputc(x,output);
                compressedBytes++;
                // increase memory by one byte
                output = realloc(output, sizeof(char) * compressedBytes);
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
        // increase memory by one byte
        output = realloc(output, sizeof(char) * compressedBytes);
        output[compressedBytes - 1] = x;
    }
    // increase memory by one byte
    output = realloc(output, sizeof(char) * (compressedBytes + 1));
    output[compressedBytes] = 0;


    /*print details of compression on the screen*/
    fprintf(stderr,"Original bits = %d\n",originalBits*8);
    fprintf(stderr,"Compressed bits = %d\n",compressedBits);
    fprintf(stderr,"Saved %.2f%% of memory\n",((float)compressedBits/(originalBits*8))*100);

    return output;
}

char *encode(const char *input, Node **tree)
{
    unsigned int codeTable[27], invCodeTable[27];

    buildHuffmanTree(tree);
    fillTable(codeTable, *tree, 0);
    invertCodes(codeTable, invCodeTable);

    return compress(input, invCodeTable);
}


/**
 * @brief Decodes the provided input 
 *
 * @param input Text to be decompressed
 * @param tree  The Huffman tree that was constructed from the original text
 */
char *decode(char const *input, const Node *tree)
{
    const Node *current = tree;
    char c,bit;
    unsigned char mask = 1 << 7;
    int j = 0;
    int i = 0, decompressedBytes = 0;
    char *output = { 0 };

    c = input[j];
    while (c != 0)
    {
        for (i=0;i<8;i++){
            bit = c & mask;
            c = c << 1;
            if (bit==0)
                current = current->left;
            else
                current = current->right;

            if(current->letter == 127)
                continue;

            char current_c;
            if (current->letter==26)
                current_c = 32;
            else
                current_c = current->letter + 97;

            decompressedBytes++;
            output = realloc(output, decompressedBytes);
            output[decompressedBytes - 1] = current_c;

            current = tree;

        }
        j++;
        c = input[j];
    }
    output = realloc(output, decompressedBytes + 1);
    output[decompressedBytes] = 0;

    return output;
}
