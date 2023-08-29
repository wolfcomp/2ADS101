#include <algorithm>
#include <chrono>
#include <iostream>
#include <ostream>
#include <random>
#include <vector>
#include <Windows.h>
#include <cstdio>

using namespace std;
using namespace std::chrono;

bool debug = false;

namespace Color {
	enum Code {
		FG_RED = 31,
		FG_GREEN = 32,
		FG_BLUE = 34,
		FG_DEFAULT = 39,
		BG_RED = 41,
		BG_GREEN = 42,
		BG_BLUE = 44,
		BG_DEFAULT = 49
	};
	class Modifier {
		Code code;
	public:
		Modifier(Code pCode) : code(pCode) {}
		friend std::ostream&
			operator<<(std::ostream& os, const Modifier& mod) {
			return os << "\033[" << mod.code << "m";
		}
	};
}

enum SortType
{
	BubbleSort,
	InsertionSort,
	SelectionSort,
	MergeSort,
	QuickSort,
	HeapSort
};

void print(const vector<int> arr, const int altColLen = -1, int start = 0, int len = -1)
{
	if (start < 0)
	{
		start = 0;
		len -= start;
	}

	const Color::Modifier red(Color::FG_RED);
	const Color::Modifier def(Color::FG_DEFAULT);

	bool altCol = false;

	if (len == -1)
		len = arr.size() - 1;

	for (int i = start; i < len; i++)
	{
		if (!altCol && altColLen != -1 && altColLen <= i)
		{
			cout << red << "' ";
			altCol = true;
		}
		cout << arr[i] << " ";
	}
	if (altCol)
		cout << def;
	cout << endl;
}

vector<int> bubble_sort(vector<int> arr)
{
	bool swapped = true;
	while (swapped)
	{
		swapped = false;
		for (int i = 0; i < arr.size() - 1; i++)
		{
			if (arr[i] > arr[i + 1])
			{
				const int temp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = temp;
				swapped = true;
			}
		}
	}
	return arr;
}

vector<int> insertion_sort(vector<int> arr)
{
	for (int i = 1; i < arr.size(); i++)
	{
		const int key = arr[i];
		if (debug)
			cout << "Sort Key: " << key << endl;
		int j = i - 1;
		while (j >= 0 && arr[j] > key)
		{
			if (debug)
				cout << "Sort: " << arr[j] << " > " << key << endl;
			arr[j + 1] = arr[j--];
		}
		if (debug)
			cout << "Sort: " << arr[j + 1] << " <= " << key << endl;
		arr[j + 1] = key;
	}
	return arr;
}

vector<int> selection_sort(vector<int> arr)
{
	for (int i = 0; i < arr.size() - 1; i++)
	{
		int min = i;
		for (int j = i + 1; j < arr.size(); j++)
		{
			if (arr[j] < arr[min])
				min = j;
		}
		if (debug)
			cout << "Min: " << min << " , arr: " << arr[min] << endl;
		if (min != i)
		{
			const int temp = arr[i];
			arr[i] = arr[min];
			arr[min] = temp;
		}
	}
	return arr;
}

vector<vector<int>> merge_divide(vector<int> arr)
{
	vector<vector<int>> result = vector<vector<int>>(2);
	result[0] = vector<int>();
	result[1] = vector<int>();
	const int half = arr.size() / 2;
	for (int i = 0; i < arr.size(); ++i)
	{
		if (i < half)
			result[0].push_back(arr[i]);
		else
			result[1].push_back(arr[i]);
	}
	return result;
}

vector<int> merge_combine(vector<vector<int>> arrs)
{
	vector<int> result;
	int i = 0, j = 0;
	const vector<int> arr1 = arrs[0];
	const vector<int> arr2 = arrs[1];
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

vector<int> merge_divide_combine(vector<vector<int>> arrs)
{
	const vector<int> arr1 = arrs[0];
	const vector<int> arr2 = arrs[1];
	if (debug)
	{
		cout << "new merge: " << endl;
		cout << "arr1: ";
		print(arr1);
		cout << "arr2: ";
		print(arr2);
	}
	if (arr1.size() > 1)
		arrs[0] = merge_divide_combine(merge_divide(arr1));
	if (arr2.size() > 1)
		arrs[1] = merge_divide_combine(merge_divide(arr2));
	if (debug)
	{
		cout << "after merge: " << endl;
		cout << "arr1: ";
		print(arr1);
		cout << "arr2: ";
		print(arr2);
	}

	return merge_combine(arrs);
}

vector<int> merge_sort(vector<int> arr)
{
	return merge_divide_combine(merge_divide(arr));
}

vector<int> quick_sort(vector<int> arr, int leftIndex, int rightIndex)
{
	if (arr.size() <= 1)
		return arr;
	int i = leftIndex, j = rightIndex;
	const int pivot = arr[leftIndex];
	if (debug)
	{
		cout << "pivot: " << pivot << endl;
		cout << "leftIndex: " << leftIndex << endl;
		cout << "rightIndex: " << rightIndex << endl;
		cout << "before: ";
		print(arr, (rightIndex - leftIndex) / 2, leftIndex, rightIndex);
	}
	while (i <= j)
	{
		while (arr[i] < pivot)
			i++;
		while (arr[j] > pivot)
			j--;
		if (i <= j)
		{
			const int temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
			i++;
			j--;
		}
	}
	if (debug)
	{
		cout << "after: ";
		print(arr, (rightIndex - leftIndex) / 2, leftIndex, rightIndex);
	}

	if (leftIndex < j)
		quick_sort(arr, leftIndex, j);
	if (i < rightIndex)
		quick_sort(arr, i, rightIndex);
	return arr;
}

vector<int> sort(const SortType sort, vector<int> arr)
{
	switch (sort)
	{
	case BubbleSort:
		return bubble_sort(arr);
	case InsertionSort:
		return insertion_sort(arr);
	case SelectionSort:
		return selection_sort(arr);
	case MergeSort:
		return merge_sort(arr);
	case QuickSort:
		return quick_sort(arr, 0, arr.size() - 1);
		//case HeapSort:
			//return heap_sort(arr);
	default:
		break;
	}
}

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	setvbuf(stdout, nullptr, _IOFBF, 1000);

	auto startArr = vector<int>();
	for (int i = 0; i < 10000; ++i)
	{
		startArr.push_back(i + 1);
	}

	cout << "Speed over array of " << startArr.size() << " elements" << endl;

	cout << "Shuffling then running Bubble sort" << endl;
	shuffle(begin(startArr), end(startArr), default_random_engine{ random_device{}() });
	auto startTime = high_resolution_clock::now();
	sort(BubbleSort, startArr);
	const auto bubbleDuration = duration_cast<milliseconds>(high_resolution_clock::now() - startTime);
	cout << "Shuffling then running Insertion sort" << endl;
	shuffle(begin(startArr), end(startArr), default_random_engine{ random_device{}() });
	startTime = high_resolution_clock::now();
	sort(InsertionSort, startArr);
	const auto insertDuration = duration_cast<microseconds>(high_resolution_clock::now() - startTime);
	cout << "Shuffling then running Selection sort" << endl;
	shuffle(begin(startArr), end(startArr), default_random_engine{ random_device{}() });
	startTime = high_resolution_clock::now();
	sort(SelectionSort, startArr);
	const auto selectionDuration = duration_cast<milliseconds>(high_resolution_clock::now() - startTime);
	cout << "Shuffling then running Merge sort" << endl;
	shuffle(begin(startArr), end(startArr), default_random_engine{ random_device{}() });
	startTime = high_resolution_clock::now();
	sort(MergeSort, startArr);
	const auto mergeDuration = duration_cast<milliseconds>(high_resolution_clock::now() - startTime);
	cout << "Shuffling then running Quick sort" << endl;
	shuffle(begin(startArr), end(startArr), default_random_engine{ random_device{}() });
	startTime = high_resolution_clock::now();
	sort(QuickSort, startArr);
	const auto quickDuration = duration_cast<milliseconds>(high_resolution_clock::now() - startTime);
	cout << "Bubble Sort: " << bubbleDuration.count() << " ms" << endl;
	cout << "Insertion Sort: " << insertDuration.count() << u8" μs" << endl;
	cout << "Selection Sort: " << selectionDuration.count() << " ms" << endl;
	cout << "Merge Sort: " << mergeDuration.count() << " ms" << endl;
	cout << "Quick Sort: " << quickDuration.count() << " ms" << endl;
	return 0;
}