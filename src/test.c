#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef HOST_COMPILE
#include <machine/patmos.h>
#endif

#include "huffman.h"
#include "measure.h"
#include "testdata.h"

static cycles_t cyc;

#ifdef ENDEBUG
#define DEBUG(...) do { fprintf(stderr, __VA_ARGS__); } while(0)
#else
#define DEBUG(...)
#endif

/**
 * @brief return Fibonacci number f_n (f_0 = 0, f_1=1)
 */
static int fib(int n)
{
    if (n==0) return 0;
    else if (n==1) return 1;
    else return fib(n-2)+fib(n-1);
}

static int run_test(const char *input, const char *test_name)
{
    int ret;
    Node *tree; //pointer to root node of tree
    char test_input[MAX_STRING_LENGTH+1];
    struct bytestream compressed;

    memset(test_input, 0, MAX_STRING_LENGTH + 1);
    strncpy(test_input, input, MAX_STRING_LENGTH);

    DEBUG("input: %s\n\n", input);
    fflush(stdout);
    fflush(stderr);

    // start measurement
#ifndef HOST_COMPILE
    clear_caches();
#endif
    MEASUREMENT_START(cyc);
    compressed = encode(test_input, &tree);
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
    //assert(!strcmp(input, decompressed));
    fprintf(stderr, "-------------------------------------\n");
    if (!strcmp(input, decompressed)) {
        fprintf(stderr, "    Test %s ran successfully!\n", test_name);
        ret = 1;
    } else {
        fprintf(stderr, "    Test %s failed!!!!\n", test_name);
        ret = 0;
    }
    fprintf(stderr, "-------------------------------------\n\n");
    //free(compressed.stream);
    //free(decompressed);

    fflush(stderr);
    fflush(stdout);

    return ret;
}


int main()
{
    char input_hello[] = "Hello World!";
    char *input;
    int input_size, idx;
    int ret = 0;

    run_test(input_hello, "hello_world");

    for (int i = 0; i < TESTDATA_LEN; i++) {
        // fprintf(stderr, "strlen(test_data[i]): %li\n", strlen(test_data[i]));
      char str[4];
      snprintf(str, 4, "%d", i);
      if (!run_test(test_data[i], str)) ret = -1;
    }

    /* limit input to ASCII characters (otherwise strcmp fails) */
    input_size = 4095;
    input = malloc(input_size+1);
    for (int i = 0; i<input_size+1; i++) {
        input[i] = i % (128-1) + 1;
    }
    input[input_size] = '\0';
    if (!run_test(input, "uniform distribution")) ret = -1;
    free(input);

    /* limit input to printable characters */
    input_size = 2583; //actual text size
    input = malloc(input_size+1); //need to add 1 for termination
    idx = 0;
    for (int i = 1; i <= 16; i++) {
        for (int j = 0; j < fib(i); j++) {
            input[idx] = i + 64;
            idx++;
        }
    }
    input[input_size] = '\0';
    if (!run_test(input, "fibonacci_16")) ret = -1;
    free(input);

    /* limit input to printable characters */
    input_size = 4095; //actual text size
    input = malloc(input_size+1); //need to add 1 for termination
    idx = 0;
    for (int i = 1; i <= 16; i++) {
        for (int j = 0; j < fib(i); j++) {
            input[idx] = i + 64;
            idx++;
        }
    }
    for (int j = 0; j < 4096-fib(18); j++) {
       input[idx] = 17 + 64;
       idx++;
    }
    input[input_size] = '\0';
    if (!run_test(input, "fibonacci_17")) ret = -1;
    free(input);

    /* limit input to ASCII characters */
    input_size = 4095; //actual text size
    input = malloc(input_size+1); //need to add 1 for termination
    idx = 0;
    for (int i = 1; i <= 16; i++) {
        for (int j = 0; j < fib(i); j++) {
            input[idx] = i;
            idx++;
        }
    }
    for (int j = 0; j < 4096-fib(18)-(127-17); j++) {
       input[idx] = 17;
       idx++;
    }
    for (int j = 18; j < 128; j++) {
        input[idx] = j;
        idx++;
    }
    assert(idx == 4095);
    input[input_size] = '\0';
    if (!run_test(input, "fibonacci_17+ones")) ret = -1;
    free(input);

    fprintf(stderr, "-------------------------------------\n");
    if (ret == 0) {
      fprintf(stderr, "  All tests ran successfully!\n");
    } else {
      fprintf(stderr, "  Some tests failed!\n");
    }
    fprintf(stderr, "-------------------------------------\n");
    return ret;
}
