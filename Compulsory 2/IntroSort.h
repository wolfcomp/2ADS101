#pragma once
#ifndef SORTBASE_H
#include "SortBase.h"
#endif // !SORTBASE_H

/**
 * \brief Implements all functions needed for IntroSort
 */
class IntroSort : public SortBase<int>
{
    /**
     * \brief Partitions the array into two arrays
     * \param arr The array to partition
     * \param left The left index
     * \param right The right index
     * \return The pivot index of the array (based on right index)
     */
    static int partition(std::vector<int>& arr, int left, int right);

    /**
     * \brief Sorts the array using insertion sort
     * \param arr The array to sort
     */
    static void insertion_sort(std::vector<int>& arr);

    /**
     * \brief Sorts the array using heap sort
     * \param arr The array to sort
     */
    static void heap_sort(std::vector<int>& arr);

    /**
     * \brief Creates a heap from the array
     * \param arr The array to create a heap from
     * \param right The index to start from
     */
    static void heapify(std::vector<int>& arr, int right);

    /**
     * \brief Sorts the array using best of heap sort, insertion sort and quick sort based on the depth/size
     * \param arr The array to sort
     * \param depth The depth of the array (2 * logarithm of the array size)
     */
    static void sort(std::vector<int>& arr, int depth);

    /**
     * \brief Searches for the first leaf in the array
     * \param arr The array to search in
     * \param left The left index
     * \param right The right index
     * \return The index of the first leaf
     */
    static int leaf_search(std::vector<int> arr, int left, int right);

    /**
     * \brief Shifts the array down
     * \param arr The array to shift
     * \param left The left index
     * \param right The right index
     */
    static void shift_down(std::vector<int>& arr, int left, int right);

    /**
     * \brief Merges two arrays into one
     * \param lower The lower array
     * \param upper The upper array
     * \return The merged array
     */
    static std::vector<int> merge(std::vector<int>& lower, std::vector<int>& upper);

public:
    /**
     * \brief Sorts the array using IntroSort
     * \param arr The array to sort
     */
    static void sort(std::vector<int>& arr);
};