#include "MergeSort.h"

std::vector<std::vector<int>> MergeSort::split(std::vector<int> arr)
{
    const auto half = arr.size() / 2;
    if (half < 1)
        return { arr };
    auto arr1 = std::vector<int>(arr.begin(), arr.begin() + half);
    auto arr2 = std::vector<int>(arr.begin() + half, arr.end());
    if(arr1.size() > 1)
        arr1 = merge(split(arr1));
    if (arr2.size() > 1)
        arr2 = merge(split(arr2));
    return { arr1, arr2 };
}

std::vector<int> MergeSort::merge(const std::vector<std::vector<int>>& arr)
{
    std::vector<int> result;
    int i = 0, j = 0;
    const std::vector<int>& arr1 = arr[0];
    const std::vector<int>& arr2 = arr[1];
    while (i < arr1.size() && j < arr2.size())
    {
        if (arr1[i] < arr2[j])
            result.push_back(arr1[i++]);
        else
            result.push_back(arr2[j++]);
    }
    while (i < arr1.size())
        result.push_back(arr1[i++]);
    while (j < arr2.size())
        result.push_back(arr2[j++]);
    return result;
}

void MergeSort::sort(std::vector<int>& arr)
{
    arr = merge(split(arr));
}