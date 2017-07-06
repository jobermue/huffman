# Huffman Coding

This repository provides an implementation of the Huffman coding algorithm for WCET analysis. 
Based on the [Huffman implementation] the code used for WCET analysis was changed to fit our needs. 
This mostly effected some algorithms and data structures. 
We were then able to run the code on the patmos processor.
One major change in comparison to the original [Huffman implementation] is the construction of the Huffman tree - we order the nodes by occurence of the respective character before combining them into the tree, which helped to decrease the overall execution time. To sort them, the `insertion sort` algorithm was used, as we already had an implementation with loop bounds and flow facts available.

## Limitations 
We have chosen to limit the length of the input string to 4096 characters (including null termination), as this is the maximum length C99 compilers support as literals.
Note that this results in a maximal codeword-length of 15 bits, a fact we used in some of our flow facts and loop bounds.
Furthermore we only allow ASCII characters (0-127) in our input string, hence the worst-case compression we can achieve is 7/8.

# Problem 1: Introduction

As a first step, a test driver and test cases were created to test the algorithm. For WCET analysis only the `encode` function was analyzed. To ensure, that the algorithm works, a string is first encoded with the Huffman algorithm, and later decoded. The result of the `decode` function was checked against the input of the `encode` function. If the output and the input match, the algorithm works correctly.

The following figure shows the execution times with pasim for `-O0`:
![Figure 1-pasim-o0n]

The following figure shows the execution times with pasim for `-O1`:
![Figure 1-pasim-o1n]

The following table summarizes the measured execution time on hardware and with `pasim` and the results of the WCET analysis with `aiT` and `platin`:

| Program  | full (`O0`) | full (`O1`) |
| -------- | ----------: | ----------: |
| hardware |  3 926 401  | 1 019 826   |
| pasim    |  6 495 269  | 1 194 240   |
| aiT      |  7 104 558  | 2 839 547   |
| platin   | 14 198 176  | 2 076 256   |

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
| hardware | 6 764 448   | 2 215 091   |
| pasim    | 9 333 186   | 2 335 286   |
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
| hardware |  3 643 421  |   715 548   |
| pasim    |  6 027 783  |   845 183   |
| aiT      |  6 072 402  | 1 902 267   |
| platin   | 12 651 121  | 1 493 517   |

## Single-Path Code

The following figure shows the execution times with pasim for `-O0` with single-path generated code:
![Figure 2-pasim-o0s]

The following figure shows the execution times with pasim for `-O1` with single-path generated code:
![Figure 2-pasim-o1s]

The following table summarizes the measured execution time on hardware and with `pasim`.
Results for the WCET analysis with `aiT` and `platin` with single-path generated code could not be obtained.

| Program  | full (`O0`) | full (`O1`) |
| -------- | ----------: | ----------: |
| hardware | 6 310 149   | 1 988 588   |
| pasim    | 8 739 631   | 2 118 512   |
| aiT      | -           | -           |
| platin   | -           | -           |

## Summary

The figures show that WCET-oriented programming can reduce the WCET and execution time variability. The same effect can be observed with single-path code.

# Problem 3: Hardware Utilization

We analyzed our code to find out which data structures are accessed frequently and would thus benefit from being put into the SPM.
Unfortunately quite a few of them did not fit into the limited size of the SPM (2kB), as can be seen in the following table:

| name                 | size[bytes]  | note |
| -------------------- | -----------: | ---- |
| `input_text`         |        4 096 | input text string |
| `pool_of_nodes`      |        6 120 |      |
| `letter_frequencies` |          256 |      |
| `array`              |        3 072 | actually an array of pointers to part of `pool_of_nodes` |
| `array_q2`           |        3 048 | actually an array of pointers to part of `pool_of_nodes` |
| `tree`               |        6 120 | actually a pointer to root node (all nodes of tree in `pool_of_nodes`) |
| `codeTable`          |          512 |      |
| `entries`            |        2 048 | form the stack used in buildHuffmanTree |
| `invCodeTable`       |          512 |      |
| `output`             |        3 584 | encoded output string |

Hence we resorted to only putting the following data structures into the SPM:
 * `codeTable`
 * `invCodeTable`
 * `letter frequencies`

We furthermore conducted a few experiments to see whether putting parts of the larger data structure into the SPM would decrease execution time, 
but all of these trials increased the execution time (see branches `spm-input` and `spm-sort`).


The following figure shows the execution times with pasim for `-O0`:
![Figure 3-pasim-o0n]

The following figure shows the execution times with pasim for `-O0` with single-path generated code:
![Figure 3-pasim-o0s]

The following table summarizes the measured execution time on hardware and with `pasim` and the results of the WCET analysis with `aiT` and `platin`:

| Program  | full (`O0`) | full, single-path (`O0`) |
| -------- | ----------: | -----------------------: |
| hardware |  3 366 478  | 6 128 324                |
| pasim    |  5 411 733  | 8 218 690                |
| aiT      |  5 530 386  | -                        |
| platin   | 11 995 514  | -                        |


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
