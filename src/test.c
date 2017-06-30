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

/**
 * @brief return Fibonacci number f_n (f_0 = 0, f_1=1)
 */
static int fib(int n)
{
    if (n==0) return 0;
    else if (n==1) return 1;
    else return fib(n-2)+fib(n-1);
}

static void run_test(const char *input)
{
    Node *tree; //pointer to root node of tree
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
    
    //free(compressed.stream);
    //free(decompressed);

    fflush(stderr);
    fflush(stdout);
}


int main()
{
    char input_hello[] = "Hello World!";
    char *input;
    int input_size, idx;

    run_test(input_hello);
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

    /* limit input to ASCII characters (otherwise strcmp fails) */
    input_size = 4095;
    input = malloc(input_size+1);
    for (int i = 0; i<input_size+1; i++) {
        input[i] = i % (128-1) + 1;
    }
    input[input_size] = '\0';
    run_test(input);
    free(input);
    fprintf(stderr, "-----------------------------------------------------\n");
    fprintf(stderr, "    Test 'uniform distribution' ran successfully!\n");
    fprintf(stderr, "-----------------------------------------------------\n");

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
    run_test(input);
    free(input);
    fprintf(stderr, "-----------------------------------------------------\n");
    fprintf(stderr, "    Test 'fibonacci_16' ran successfully!\n");
    fprintf(stderr, "-----------------------------------------------------\n");


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
    run_test(input);
    free(input);
    fprintf(stderr, "-----------------------------------------------------\n");
    fprintf(stderr, "    Test 'fibonacci_17' ran successfully!\n");
    fprintf(stderr, "-----------------------------------------------------\n");

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
    run_test(input);
    free(input);
    fprintf(stderr, "-----------------------------------------------------\n");
    fprintf(stderr, "    Test 'fibonacci_17+1s' ran successfully!\n");
    fprintf(stderr, "-----------------------------------------------------\n");

    fprintf(stderr, "-------------------------------------\n");
    fprintf(stderr, "  All tests ran successfully!\n");
    fprintf(stderr, "-------------------------------------\n");
    return 0;
}
