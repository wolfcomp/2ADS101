#pragma once
#ifndef SORTBASE_H
#include "SortBase.h"
#endif // !SORTBASE_H

/**
 * \brief Implements all functions needed for MergeSort
 */
class MergeSort : public SortBase<int>
{
    /**
     * \brief merges the two arrays into one sorted array
     * \param arr The two arrays to merge
     * \return The merged array
     */
    static std::vector<int> merge(const std::vector<std::vector<int>>& arr);

    /**
     * \brief Splits the array into two arrays
     * \param arr The array to split
     * \return The two arrays half of the original array
     */
    static std::vector<std::vector<int>> split(std::vector<int> arr);
public:
    /**
     * \brief Sorts the array using MergeSort
     * \param arr The array to sort
     */
    static void sort(std::vector<int> &arr);
};