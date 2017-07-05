/**
 * @brief Interface for merge sort
 */

#ifndef MERGE_SORT_H
#define MERGE_SORT_H


#if defined(__PATMOS__) && defined(USE_SPM)

#include <machine/spm.h>

void merge_sort_nrecursive(_SPM lfreq_t *arr, int size);

#else /* __PATMOS__ */

void merge_sort_nrecursive(lfreq_t *arr, int size);

#endif /* __PATMOS__ */


#endif
