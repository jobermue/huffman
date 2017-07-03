/*
 * insertion_sort.c
 *
 * Daniel Prokesch <daniel@vmars.tuwien.ac.at>, 2014-2017
 * Benedikt Huber <benedikt@vmars.tuwien.ac.at>, 2010-2017
 *
 * WCET Analysis Lab
 * Cyber-Physical Systems Group
 * Institute of Computer Engineering
 * Vienna University of Technology
 *
 * insertion sort routine
 */

#include "huffman.h"


/**
 * @brief Sort given array of integers using insertion sort
 * @param arr  array of integers
 * @param size number of elements in the array (user register upper bound: @size)
 */
/* ai: instruction insertion_sort is entered with @size = 128;  */
__attribute__((noinline))
void insertion_sort(Node **arr, int size /* passed in r4 */)
{
  int i,j;
  Node *current;
  j = 1;

#ifndef HOST_COMPILE
  __llvm_pcmarker(0);
#endif

  /* ai: loop (here) max @size-1; */
  #pragma loopbound min 0 max 126
  while (j < size)
    {
      /* invariant: sorted (a[0..j-1]) */
      current = arr[j];

      i = j - 1;

      /* ai: loop (here) max @size-1;  */
      #pragma loopbound min 0 max 126
      while (i >= 0)
        {
          /* ai?: flow (here) <= (@size * (@size-1) / 2) ("insertion_sort"); */
          /* Unfortunately, aiT is not capable of interpreting parametric flow facts */
          /* size | flow */
          /* 127  | 8001 */
          /* 128  | 8128 */
          /* ai: flow (here) <= 8128 ("insertion_sort"); */
#ifndef HOST_COMPILE
          __llvm_pcmarker(1);
#endif
          if (arr[i]->value < current->value) break;
          arr[i+1] = arr[i]; /* ai: LABEL here = "insertion_sort_move"; */
          i = i - 1;
        }

      arr[i+1] = current;
      j = j + 1;
    }
}
