#include "BubbleSort.h"

void BubbleSort::sort(std::vector<int>& arr)
{
    bool swapped = true;
    int j = 0;
    while (swapped)
    {
        swapped = false;
        j++;
        for (int i = 0; i < arr.size() - j; i++)
        {
            if (arr[i] > arr[i + 1])
            {
                swap(arr, i, i + 1);
                swapped = true;
            }
        }
    }
}
