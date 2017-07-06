# huffman

Implementation of Huffman Coding Algorithm for WCET analysis. Based on the [Huffman implementation] the code used for WCET analysis was changed to fit our needs. This motly effected some algorithms and data structures. Having the code on this basis, we were able to run the code on the hardware.
Despite the original [Huffman implementation] the nodes of the tree were ordered according to their occurence. This helped to decrese the overall execution time. To sort them, the `insertion sort` algorithm was used.

## max length of strings
testdata.h:24:1: warning: string length ‘4096’ is greater than the length ‘4095’ ISO C99 compilers are required to support [-Woverlength-strings]

-> hence we have chosen to limit the length of the input string to 4095 characters (not including null termination).

-> this results in a maximal code length of 15 bits

# Problem 1: Introduction

As a first step, a test driver and test cases were created to test the algorithm. For WCET analysis only the `encode` function was analyzed. To ensure, that the algorithm works, a string is first encoded with the Huffman algorithm, and later decoded. The result of the `decode` function was checked against the input of the `encode` function. If the output and the input match, the algorithm works correctly.

The following figure shows the execution times with pasim for `-O0`:
![Figure 1-pasim-o0n]

The following figure shows the execution times with pasim for `-O1`:
![Figure 1-pasim-o1n]

The following table summarizes the measured execution time on hardware and with `pasim` and the results of the WCET analysis with `aiT` and `platin`:

| Program  | full (`O0`) | full (`O1`) |
| -------- | ----------: | ----------: |
| hardware | 3926401     | 1019826     |
| pasim    | 6495269     | 1194240     |
| aiT      | 7104558     | 2839547     |
| platin   | 14198176    | 2076256     |

## Single-Path Code

The single-path code was generated with the option `-mpatmos-singlepath=encode`.


The following figure shows the execution times with pasim for `-O0` with single-path generated code:
![Figure 1-pasim-o0s]

The following figure shows the execution times with pasim for `-O1` with single-path generated code:
![Figure 1-pasim-o1s]

The following table summarizes the measured execution time on hardware and with `pasim`.
Results for the WCET analysis with `aiT` and `platin` with single-path generated code could not be obtained.

| Program  | full (`O0`) | full (`O1`) |
| -------- | ----------: | ----------: |
| hardware | 6764448     | 2215091     |
| pasim    | 9333186     | 2335286     |
| aiT      | -           | -           |
| platin   | -           | -           |

## Summary

As the figures above show, applying a single-path generator increases the WCET, but reduces the variance of the execution times, which makes it more predictable.


# Problem 2: WCET-Oriented Programming

In WCET-oriented programming the aim is to make the algorithm as independent from the input as possible. This results in execution times that do not differ too much from each other, as they increase and move towards the WCET. The positive effect is that the WCET decreases compared to non-WCET oriented implementations.

To achive this goal we replaced the `insertion sort` algorithm with the `merge sort` algorithm, which has better WCET and less execution time variance. We took a [non-recursive merge sort] algorithm to be able to analyze the WCETs with the given tools.
Another WCET-oriented improvement is to work with an input string of fixed lenth (4096). The remaining string is filled with `0` and are processed. Furthermore we build a huffman tree containing all characters (although possibly with a frequency value of 0), not just those present in the input.
Whenever possible we replaced `while` and `for` loops, with variable variable loop iterations, by `for` loops with a fixed number of loop iterations.

The following figure shows the execution times with pasim for `-O0`:
![Figure 2-pasim-o0n]

The following figure shows the execution times with pasim for `-O1`:
![Figure 2-pasim-o1n]

The following table summarizes the measured execution time on hardware and with `pasim` and the results of the WCET analysis with `aiT` and `platin`:

| Program  | full (`O0`) | full (`O1`) |
| -------- | ----------: | ----------: |
| hardware | 3643421     | 715548      |
| pasim    | 6027783     | 845183      |
| aiT      | 6072402     | 1902267     |
| platin   | 12651121    | 1493517     |

## Single-Path Code

The following figure shows the execution times with pasim for `-O0` with single-path generated code:
![Figure 2-pasim-o0s]

The following figure shows the execution times with pasim for `-O1` with single-path generated code:
![Figure 2-pasim-o1s]

The following table summarizes the measured execution time on hardware and with `pasim`.
Results for the WCET analysis with `aiT` and `platin` with single-path generated code could not be obtained.

| Program  | full (`O0`) | full (`O1`) |
| -------- | ----------: | ----------: |
| hardware | 6310149     | 1988588     |
| pasim    | 8739631     | 2118512     |
| aiT      | -           | -           |
| platin   | -           | -           |

## Summary

The figures show that WCET-oriented programming can reduce the WCET and execution time variability. The same effect can be observed with single-path code.

# Problem 3: Hardware Utilization

The following figure shows the execution times with pasim for `-O0`:
![Figure 3-pasim-o0n]

The following figure shows the execution times with pasim for `-O0` with single-path generated code:
![Figure 3-pasim-o0s]

The following table summarizes the measured execution time on hardware and with `pasim` and the results of the WCET analysis with `aiT` and `platin`:

| Program  | full (`O0`) | full, single-path (`O0`) |
| -------- | ----------: | -----------------------: |
| hardware | 3366478     | 6128324                  |
| pasim    | 5411733     | 8218690                  |
| aiT      | 5530386     | -                        |
| platin   | 11995514    | -                        |

[Huffman implementation]: http://www.programminglogic.com/implementing-huffman-coding-in-c/
[non-recursive merge sort]: https://stackoverflow.com/questions/1557894/non-recursive-merge-sort#17957133
[Figure 1-pasim-o0n]: ./results/plots/1-pasim-full.csv-o0n.jpg
[Figure 1-pasim-o1n]: ./results/plots/1-pasim-full.csv-o1n.jpg
[Figure 1-pasim-o0s]: ./results/plots/1-pasim-full.csv-o0s.jpg
[Figure 1-pasim-o1s]: ./results/plots/1-pasim-full.csv-o1s.jpg
[Figure 2-pasim-o0n]: ./results/plots/2-pasim-full.csv-o0n.jpg
[Figure 2-pasim-o1n]: ./results/plots/2-pasim-full.csv-o1n.jpg
[Figure 2-pasim-o0s]: ./results/plots/2-pasim-full.csv-o0s.jpg
[Figure 2-pasim-o1s]: ./results/plots/2-pasim-full.csv-o1s.jpg
[Figure 3-pasim-o0n]: ./results/plots/3-pasim-full.csv-o0n.jpg
[Figure 3-pasim-o0s]: ./results/plots/3-pasim-full.csv-o0s.jpg
