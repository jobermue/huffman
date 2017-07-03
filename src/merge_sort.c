/*
 * taken from http://www.cprogramming.com/tutorial/computersciencetheory/merge.html
 */

#include <assert.h>
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
/* ai: instruction merge_sort is entered with @size = 127;  */
__attribute__((noinline))
void merge_sort(Node **arr, int size)
{
    assert(size <= 127);
    Node *scratch[127];

    merge_helper(arr, 0, size, scratch);
}

__attribute__((noinline))
void merge_sort_nrecursive(Node **a, int num)
{
    assert(num <= 127);
    Node *b[127];

    int rght, wid, rend;
    int i, j, m, t;

    for(int k=1; k < num; k *= 2)
    {
        for(int left=0; left+k < num; left += 2*k)
        {
            rght = left + k;
            rend = rght + k;
            if(rend > num) rend = num;
            while(i < rght && j < rend)
            {
                if(a[i] <= a[j])
                {
                    b[m] = a[i];
                    i++;
                }
                else
                {
                    b[m] = a[j];
                    j++;
                }
                m++;
            }
            while(i < rght)
            {
                b[m] = a[i];
                i++;
                m++;
            }
            while(i < rend)
            {
                b[m] = a[j];
                m++;
            }
            for(m = left; m < rend; m++)
            {
                a[m] = b[m];
            }
        }
    }
}

