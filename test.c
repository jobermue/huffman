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
}


int main()
{
    char input[] = "Hello World!";

    run_test(input);

    for (int i = 0; i < TESTDATA_LEN; i++) {
        run_test(test_data[i]);
        printf("-------------------------------------\n");
        printf("    Test %i ran successfully!\n", i);
        printf("-------------------------------------\n");
    }

    printf("-------------------------------------\n");
    printf("  All tests ran successfully!\n");
    printf("-------------------------------------\n");
    return 0;
}
