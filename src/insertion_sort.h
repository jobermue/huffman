/*
 * insertion_sort.h
 *
 * Daniel Prokesch <daniel@vmars.tuwien.ac.at>, 2014-2016
 * Benedikt Huber <benedikt@vmars.tuwien.ac.at>, 2010-2016
 *
 * WCET Analysis Lab
 * Cyber-Physical Systems Group
 * Institute of Computer Engineering
 * Vienna University of Technology
 *
 * insertion sort routine
 */

//! @brief Standard insertion sort implementation
__attribute__((noinline))
void insertion_sort(Node **arr, int size);

