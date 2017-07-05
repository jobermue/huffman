/*
 * taken from http://www.cprogramming.com/tutorial/computersciencetheory/merge.html
 */

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include "huffman.h"

#if defined(__PATMOS__) && defined(USE_SPM)
#include <machine/spm.h>
#endif /* __PATMOS__ */


/**
 * @brief Sort given array of integers using non-recursive merge sort
 * (see https://stackoverflow.com/questions/1557894/non-recursive-merge-sort#17957133)
 *
 * @param input  array of integers
 * @param size   number of elements in the array (user register upper bound: @size)
 */
/* ai: instruction merge_sort_nrecursive is entered with @size = 128;  */
#if defined(__PATMOS__) && defined(USE_SPM)
void merge_sort_nrecursive(_SPM lfreq_t *arr, int size)
#else /* __PATMOS__ */
void merge_sort_nrecursive(lfreq_t *arr, int size)
#endif /* __PATMOS__ */
{
    //assert(size <= NR_OF_CHARS);
    lfreq_t scratch[NR_OF_CHARS];

    uint16_t rght, rend;
    uint16_t i=0, j=0, m=0;

    #pragma loopbound min 1 max 7
    for(uint16_t k=1; k < size; k *= 2)
    {
        /* ai: LABEL here = "ms_loop_1"; */
#ifndef HOST_COMPILE
        __llvm_pcmarker(8);
#endif
        #pragma loopbound min 0 max 64
        for(uint16_t left=0; left+k < size; left += 2*k)
        {
            /* ai: LABEL here = "ms_loop_2"; */
#ifndef HOST_COMPILE
            __llvm_pcmarker(9);
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
                __llvm_pcmarker(10);
#endif
                if(arr[i].freq <= arr[j].freq)
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
                __llvm_pcmarker(11);
#endif
                scratch[m] = arr[i];
                i++;
                m++;
            }
            while(j < rend)
            {
                /* ai: LABEL here = "ms_loop_5"; */
#ifndef HOST_COMPILE
                __llvm_pcmarker(12);
#endif
                scratch[m] = arr[j];
                j++;
                m++;
            }

            for(m = left; m < rend; m++)
            {
                /* ai: LABEL here = "ms_loop_6"; */
#ifndef HOST_COMPILE
                __llvm_pcmarker(13);
#endif
                arr[m] = scratch[m];
            }
        }
    }
}

