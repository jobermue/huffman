# huffman

Implementation of Huffman Coding Algorithm for WCET analysis. Based on the [Huffman implementation] the code used for WCET analysis was changed to fit our needs. This motly effected some algorithms and data structures. Having the code on this basis, we were able to run the code on the hardware.
Despite the original [Huffman implementation] the nodes of the tree were ordered according to their occurence. This helped to decrese the overall execution time. To sort them, the `insertion sort` algorithm was used.

## max length of strings
testdata.h:24:1: warning: string length ‘4096’ is greater than the length ‘4095’ ISO C99 compilers are required to support [-Woverlength-strings]

-> hence we have chosen to limit the length of the input string to 4095 characters (not including null termination).

-> this results in a maximal code length of 15 bits

# Problem 1: Introduction

As a first step, a test driver and test cases were created to test the algorithm. For WCET analysis only the `encode` function was analyzed. To ensure, that the algorithm works, a string is first encoded with the Huffman algorithm, and later decoded. The result of the `decode` function was checked against the input of the `encode` function. If the output and the input match, the algorithm works correctly.

The following figure shows the execution times on hardware for `O0`:
![Figure 1]

The following figure shows the execution times on hardware for `O1`:
![Figure 2]

The following figure shows the execution times on hardware for `O2`:
![Figure 3]

The following table summarizes the WCET analysis with `pasim`, `aiT` and `patmos`:

| Program | local | dcideal | full |
| ------- | ----- | ------- | ---- |
| pasim   | TBA   | TBA     | TBA  |
| aiT     | TBA   | TBA     | TBA  |
| patmos  | TBA   | TBA     | TBA  |

## Single-Path Code

The single-path code was generated with the option `-mpatmos-singlepath=<your_function>`.


The following figure shows the execution times on hardware for `O0` witch single-path generated code:
![Figure 4]

The following figure shows the execution times on hardware for `O1` witch single-path generated code:
![Figure 5]

The following figure shows the execution times on hardware for `O2` witch single-path generated code:
![Figure 6]

The following table summarizes the WCET analysis with `pasim`, `aiT` and `patmos` witch single-path generated code:

| Program | local | dcideal | full |
| ------- | ----- | ------- | ---- |
| pasim   | TBA   | TBA     | TBA  |
| aiT     | TBA   | TBA     | TBA  |
| patmos  | TBA   | TBA     | TBA  |

## Summary

As the figures above show, applying a single-path generator decreases the WCET, but also reduces the variance of the execution times, which makes it more predictable.


# Problem 2: WCET-Oriented Programming

In WCET-oriented programming the aim is to make the algorithm as independent from the input as possible. This results in execution times that do not differ too much from each other, as they increase and move towards the WCET. The positive effect is that the WCET decreases compared to non-WCET oriented implementations.

To achive this goal we replaced the `insertion sort` algorithm by the `merge sort`, which has worse BCET, but better WCET. We took a [non-recursive merge sort] algorithm to be able to analyze the WCETs.


The following figure shows the execution times on hardware for `O0`:
![Figure 7]

The following figure shows the execution times on hardware for `O1`:
![Figure 8]

The following figure shows the execution times on hardware for `O2`:
![Figure 9]

The following table summarizes the WCET analysis with `pasim`, `aiT` and `patmos`:

| Program | local | dcideal | full |
| ------- | ----- | ------- | ---- |
| pasim   | TBA   | TBA     | TBA  |
| aiT     | TBA   | TBA     | TBA  |
| patmos  | TBA   | TBA     | TBA  |

The following figure shows the execution times on hardware for `O0` with single-path generated code:
![Figure 10]

The following figure shows the execution times on hardware for `O1` with single-path generated code:
![Figure 11]

The following figure shows the execution times on hardware for `O2` with single-path generated code:
![Figure 12]

The following table summarizes the WCET analysis with `pasim`, `aiT` and `patmos` witch single-path generated code:

| Program | local | dcideal | full |
| ------- | ----- | ------- | ---- |
| pasim   | TBA   | TBA     | TBA  |
| aiT     | TBA   | TBA     | TBA  |
| patmos  | TBA   | TBA     | TBA  |

# Problem 3: Hardware Utilization

[Huffman implementation]: http://www.programminglogic.com/implementing-huffman-coding-in-c/
[non-recursive merge sort]: https://stackoverflow.com/questions/1557894/non-recursive-merge-sort#17957133
[Figure 1]: ./results/plots/1/o0n.png
[Figure 2]: ./results/plots/1/o1n.png
[Figure 3]: ./results/plots/1/o2n.png
[Figure 4]: ./results/plots/1/o0s.png
[Figure 5]: ./results/plots/1/o1s.png
[Figure 6]: ./results/plots/1/o2s.png
[Figure 7]: ./results/plots/2/o0n.png
[Figure 8]: ./results/plots/2/o1n.png
[Figure 9]: ./results/plots/2/o2n.png
[Figure 10]: ./results/plots/2/o0s.png
[Figure 11]: ./results/plots/2/o1s.png
[Figure 12]: ./results/plots/2/o2s.png
