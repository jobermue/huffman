#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffman.h"
#include "measure.h"
#include "testdata.h"

static cycles_t cyc;


static void run_test(const char *input)
{
    Node *tree;
    struct bytestream compressed;

    printf("input: %s\n\n", input);
    fflush(stdout);

    // start measurement
    MEASUREMENT_START(cyc);
    compressed = encode(input, &tree);
    // stop measurement
    MEASUREMENT_STOP(cyc);
    MEASUREMENT_DUMP(cyc);

    char *decompressed = decode(compressed, tree);

    // compare decompressed with original
    printf("compressed:");
    for (int i = 0; i*8 < compressed.len; i++) {
        printf(" %02x", (unsigned int) compressed.stream[i]);
    }
    printf("\n");
    printf("original text: %s\n", input);
    printf("decompressed:  %s\n", decompressed);
    assert(!strcmp(input, decompressed));
    
    free(compressed.stream);
    free(decompressed);

    fflush(stderr);
    fflush(stdout);
}


int main()
{
    char input[] = "Hello World!";

    run_test(input);
    printf("--------------------------------------------\n");
    printf("    Test 'hello_world' ran successfully!\n");
    printf("--------------------------------------------\n");
    fflush(stdout);

    for (int i = 0; i < TESTDATA_LEN; i++) {
        // printf("strlen(test_data[i]): %li\n", strlen(test_data[i]));
        run_test(test_data[i]);
        printf("-------------------------------------\n");
        printf("    Test %i ran successfully!\n", i);
        printf("-------------------------------------\n\n");
        fflush(stdout);
    }

    /* limit input to printable characters */
    char input2[4096];
    for (int i = 0; i<4096; i++) {
        input2[i] = i % (127-32) + 32;
    }
    input2[4095] = '\0';
    run_test(input2);
    printf("-----------------------------------------------------\n");
    printf("    Test 'uniform distribution' ran successfully!\n");
    printf("-----------------------------------------------------\n");

    printf("-------------------------------------\n");
    printf("  All tests ran successfully!\n");
    printf("-------------------------------------\n");
    return 0;
}
