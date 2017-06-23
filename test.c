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

    // start measurement
    MEASUREMENT_START(cyc);
    char *compressed = encode(input, &tree);
    // stop measurement
    MEASUREMENT_STOP(cyc);
    MEASUREMENT_DUMP(cyc);

    char *decompressed = decode(compressed, tree);

    // compare decompressed with original
    printf("original text: %s\n", input);
    printf("decompressed:  %s\n", decompressed);
    assert(!strcmp(input, decompressed));
    
    free(compressed);
    free(decompressed);
}


int main()
{
    char input[] = "hello world";

    run_test(input);

    for (int i = 0; i < TESTDATA_LEN; i++) {
        run_test(test_data[i]);
    }

    return 0;
}
