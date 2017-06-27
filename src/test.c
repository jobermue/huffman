#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffman.h"
#include "measure.h"
#include "testdata.h"

static cycles_t cyc;

#ifdef ENDEBUG
#define DEBUG(...) do { fprintf(stderr, __VA_ARGS__); } while(0)
#else
#define DEBUG(...)
#endif


static void run_test(const char *input)
{
    Node *tree;
    struct bytestream compressed;

    DEBUG("input: %s\n\n", input);
    fflush(stdout);
    fflush(stderr);

    // start measurement
    MEASUREMENT_START(cyc);
    compressed = encode(input, &tree);
    // stop measurement
    MEASUREMENT_STOP(cyc);
    MEASUREMENT_DUMP(cyc);

    char *decompressed = decode(compressed, tree);

    // compare decompressed with original
    DEBUG("compressed:");
    for (int i = 0; i*8 < compressed.len; i++) {
        DEBUG(" %02x", (unsigned int) compressed.stream[i]);
    }
    DEBUG("\n");
    DEBUG("original text: %s\n", input);
    DEBUG("decompressed:  %s\n", decompressed);
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
    fprintf(stderr, "--------------------------------------------\n");
    fprintf(stderr, "    Test 'hello_world' ran successfully!\n");
    fprintf(stderr, "--------------------------------------------\n");
    fflush(stdout);

    for (int i = 0; i < TESTDATA_LEN; i++) {
        // fprintf(stderr, "strlen(test_data[i]): %li\n", strlen(test_data[i]));
        run_test(test_data[i]);
        fprintf(stderr, "-------------------------------------\n");
        fprintf(stderr, "    Test %i ran successfully!\n", i);
        fprintf(stderr, "-------------------------------------\n\n");
        fflush(stdout);
    }

    /* limit input to printable characters */
    char input2[4096];
    for (int i = 0; i<4096; i++) {
        input2[i] = i % (127-32) + 32;
    }
    input2[4095] = '\0';
    run_test(input2);
    fprintf(stderr, "-----------------------------------------------------\n");
    fprintf(stderr, "    Test 'uniform distribution' ran successfully!\n");
    fprintf(stderr, "-----------------------------------------------------\n");

    fprintf(stderr, "-------------------------------------\n");
    fprintf(stderr, "  All tests ran successfully!\n");
    fprintf(stderr, "-------------------------------------\n");
    return 0;
}
