#pragma once
#ifndef SORTBASE_H
#include "SortBase.h"
#endif // !SORTBASE_H
/**
 * \brief Implements all functions required for BubbleSort
 */
class BubbleSort : public SortBase<int>
{
public:
    /**
     * \brief Sorts the array using BubbleSort
     * \param arr The array to sort
     */
    static void sort(std::vector<int>& arr);
};