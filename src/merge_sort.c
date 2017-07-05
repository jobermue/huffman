/*
 * taken from http://www.cprogramming.com/tutorial/computersciencetheory/merge.html
 */

#include <assert.h>
#include <stdint.h>
#include "huffman.h"

/* Helper function for finding the max of two Nodes */
Node *max(Node *x, Node *y)
{
    if(x->value > y->value) {
        return x;
    } else {
        return y;
    }
}

/* left is the index of the leftmost element of the subarray; right is one
 * past the index of the rightmost element */
void merge_helper(Node **input, int left, int right, Node **scratch)
{
    /* base case: one element */
    if (right == left + 1) {
        return;
    } else {
        int i = 0;
        int length = right - left;
        int midpoint_distance = length/2;
        /* l and r are to the positions in the left and right subarrays */
        int l = left, r = left + midpoint_distance;

        /* sort each subarray */
        merge_helper(input, left, left + midpoint_distance, scratch);
        merge_helper(input, left + midpoint_distance, right, scratch);

        /* merge the arrays together using scratch for temporary storage */ 
        for (i = 0; i < length; i++) {
            /* Check to see if any elements remain in the left array; if so,
             * we check if there are any elements left in the right array; if
             * so, we compare them.  Otherwise, we know that the merge must
             * use take the element from the left array */
            if (l < left + midpoint_distance && 
                    (r == right || max(input[l], input[r]) == input[l]))
            {
                scratch[i] = input[l];
                l++;
            } else {
                scratch[i] = input[r];
                r++;
            }
        }
        /* Copy the sorted subarray back to the input */
        for(i = left; i < right; i++) {
            input[i] = scratch[i - left];
        }
    }
}

/* mergesort returns true on success.  Note that in C++, you could also
 * replace malloc with new and if memory allocation fails, an exception will
 * be thrown.  If we don't allocate a scratch array here, what happens? 
 *
 * Elements are sorted in reverse order -- greatest to least */


/**
 * @brief Sort given array of integers using insertion sort
 *
 * @param input  array of integers
 * @param size   number of elements in the array (user register upper bound: @size)
 */
/* ai: instruction merge_sort is entered with @size = 128;  */
__attribute__((noinline))
void merge_sort(Node **arr, int size)
{
    assert(size <= NR_OF_CHARS);
    Node *scratch[NR_OF_CHARS];

    merge_helper(arr, 0, size, scratch);
}

/**
 * @brief Sort given array of integers using non-recursive merge sort
 * (see https://stackoverflow.com/questions/1557894/non-recursive-merge-sort#17957133)
 *
 * @param input  array of integers
 * @param size   number of elements in the array (user register upper bound: @size)
 */
/* ai: instruction merge_sort_nrecursive is entered with @size = 128;  */
__attribute__((noinline))
void merge_sort_nrecursive(Node **arr, int size)
{
    //assert(size <= NR_OF_CHARS);
    Node *scratch[NR_OF_CHARS];

    uint16_t rght, rend;
    uint16_t i=0, j=0, m=0;

    #pragma loopbound min 1 max 7
    for(uint16_t k=1; k < size; k *= 2)
    {
        /* ai: label here = "ms_loop_1"; */
        #pragma loopbound min 0 max 64
        for(uint16_t left=0; left+k < size; left += 2*k)
        {
            /* ai: label here = "ms_loop_2"; */
#ifndef HOST_COMPILE
            __llvm_pcmarker(8);
#endif
            rght = left + k;
            rend = rght + k;
            if(rend > size)
            {
                rend = size;
            } 

            m = left;
            i = left;
            j = rght;

            while(i < rght && j < rend)
            {
                /* ai: LABEL here = "ms_loop_3"; */
#ifndef HOST_COMPILE
                __llvm_pcmarker(9);
#endif
                if(arr[i] <= arr[j])
                {
                    scratch[m] = arr[i];
                    i++;
                }
                else
                {
                    scratch[m] = arr[j];
                    j++;
                }
                m++;
            }
            while(i < rght)
            {
                /* ai: LABEL here = "ms_loop_4"; */
#ifndef HOST_COMPILE
                __llvm_pcmarker(10);
#endif
                scratch[m] = arr[i];
                i++;
                m++;
            }
            while(j < rend)
            {
                /* ai: LABEL here = "ms_loop_5"; */
#ifndef HOST_COMPILE
                __llvm_pcmarker(11);
#endif
                scratch[m] = arr[j];
                j++;
                m++;
            }

            for(m = left; m < rend; m++)
            {
                /* ai: LABEL here = "ms_loop_6"; */
#ifndef HOST_COMPILE
                __llvm_pcmarker(12);
#endif
                arr[m] = scratch[m];
            }
        }
    }
}

