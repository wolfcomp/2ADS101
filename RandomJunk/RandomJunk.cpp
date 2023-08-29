#include <algorithm>
#include <chrono>
#include <iostream>
#include <ostream>
#include <random>
#include <vector>
#include <Windows.h>
#include <cstdio>
#include <iomanip>
#include <string>

using namespace std;
using namespace std::chrono;

bool debug = false;
char separator = ' ';

template<typename T> void printElement(T t, const int& width, bool rightAlign = false)
{
	if (rightAlign)
		cout << right << setw(width) << setfill(separator) << t;
	else
		cout << left << setw(width) << setfill(separator) << t;
}

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
	CocktailSort,
	HeapSort
};

std::ostream& operator<<(std::ostream& out, const SortType value) {
	return out << [value] {
#define PROCESS_VAL(p) case(p): return #p;
		switch (value) {
			PROCESS_VAL(BubbleSort)
				PROCESS_VAL(InsertionSort)
				PROCESS_VAL(SelectionSort)
				PROCESS_VAL(MergeSort)
				PROCESS_VAL(QuickSort)
				PROCESS_VAL(CocktailSort)
				PROCESS_VAL(HeapSort)
		}
#undef PROCESS_VAL
		return "";
	}();
}

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
		len = arr.size();

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

void swap(vector<int>& arr, int i, int j)
{
	if (debug)
		cout << "Swap: " << arr[i] << " <-> " << arr[j] << endl;
	const int temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
}

void swap(int& i, int& j)
{
	if (debug)
		cout << "Swap: " << i << " <-> " << j << endl;
	const int temp = i;
	i = j;
	j = temp;
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
				swap(arr, i, i + 1);
				swapped = true;
			}
		}
	}
	return arr;
}

vector<int> insertion_sort(vector<int> arr)
{
	int i = 1;
	while (i < arr.size())
	{
		int j = i;
		while (j > 0 && arr[j - 1] > arr[j])
		{
			swap(arr, j, j - 1);
			j--;
		}
		i++;
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
			swap(arr, i, min);
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

int partition(vector<int>& arr, int lo, int hi)
{
	const int pivot = arr[(hi - lo) / 2 + lo];
	int i = lo - 1, j = hi + 1;
	while (true)
	{
		do i++;	while (arr[i] < pivot);
		do j--;	while (arr[j] > pivot);
		if (i >= j) return j;
		swap(arr, i, j);
	}
}

vector<int> quick_sort(vector<int> arr, int lo, int hi)
{
	if (arr.size() <= 1)
		return arr;
	if (debug)
	{
		cout << "lo: " << lo << endl;
		cout << "hi: " << hi << endl;
	}
	if (lo >= 0 && hi >= 0 && lo < hi)
	{
		int p = partition(arr, lo, hi);
		arr = quick_sort(arr, lo, p);
		arr = quick_sort(arr, p + 1, hi);
	}
	return arr;
}

vector<int> cocktail_sort(vector<int> arr)
{
	int lower = 0, upper = arr.size() - 1;
	while (lower <= upper)
	{
		for (int i = lower; i < arr.size() - 1; ++i)
		{
			if (arr[i] > arr[i + 1])
			{
				swap(arr, i, i + 1);
			}
		}
		for (int i = upper; i > lower; --i)
		{
			if (arr[i - 1] > arr[i])
			{
				swap(arr, i - 1, i);
			}
		}
		lower++;
		upper--;
	}
	return arr;
}

int leaf_search(vector<int> arr, int i, int rightIndex)
{
	int j = i;
	while (2 * j + 2 <= rightIndex)
	{
		if (arr[2 * j + 2] > arr[2 * j + 1])
			j = 2 * j + 2;
		else
			j = 2 * j + 1;
	}
	if (2 * j + 1 <= rightIndex)
		j = 2 * j + 1;
	return j;
}

void shift_down(vector<int>& arr, int i, int rightIndex)
{
	int j = leaf_search(arr, i, rightIndex);
	while (arr[i] > arr[j])
		j = (j - 1) / 2;
	int temp = arr[j];
	arr[j] = arr[i];
	while (j > i)
	{
		int p = (j - 1) / 2;
		swap(temp, arr[p]);
		j = p;
	}
}

void heapify(vector<int>& arr, int rightIndex)
{
	int start = (rightIndex - 2) / 2;
	while (start >= 0)
	{
		shift_down(arr, start, rightIndex - 1);
		start--;
	}
}

vector<int> heap_sort(vector<int> arr)
{
	int count = arr.size();
	heapify(arr, count--);
	while (count > 0)
	{
		swap(arr, 0, count--);
		shift_down(arr, 0, count);
	}
	return arr;
}

vector<int> sort(const SortType type, vector<int> arr)
{
	switch (type)
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
	case CocktailSort:
		return cocktail_sort(arr);
	case HeapSort:
		return heap_sort(arr);
	default:
		break;
	}
}

auto rnd = default_random_engine{ random_device{}() };

vector<double> time(const SortType type, int size, int times = 10)
{
	vector<double> result = vector<double>(3);
	result[0] = -1;
	vector<int> arr = vector<int>();
	vector<int> sorted;
	for (int i = 0; i < size; ++i)
	{
		arr.push_back(i + 1);
	}
	shuffle(begin(arr), end(arr), rnd);
	for (int i = 0; i < times; i++)
	{
		cout << "Running " << type << " of size " << size << " , " << i + 1 << " out of " << times << " times" << endl;
		auto startTime = high_resolution_clock::now();
		sorted = sort(type, arr);
		const double duration = static_cast<double>(duration_cast<microseconds>(high_resolution_clock::now() - startTime).count());
		result[1] += duration;
		if (result[0] < 0 || result[0] > duration)
			result[0] = duration;
		if (result[2] < duration)
			result[2] = duration;
		if (i != times - 1)
			shuffle(begin(arr), end(arr), rnd);
	}
	print(sorted);
	result[1] /= times;
	return result;
}

int nameLen = 0;
int minLen = 0;
int maxLen = 0;
int avgLen = 0;

string format_number(long long num)
{
	string result = to_string(num);
	int i = result.length() - 3;
	while (i > 0)
	{
		result.insert(i, ",");
		i -= 3;
	}
	return result;
}

void formatTime(string name, vector<double> time, bool dry = false)
{
	const auto avg = time[1];
	const auto avgDec = avg - static_cast<long long>(avg);
	auto avgDecStr = to_string(avgDec);
	avgDecStr = avgDecStr.substr(avgDecStr.find('.') + 1, 3);
	avgDecStr = avgDecStr.substr(0, avgDecStr.find_last_not_of('0') + 1);

	auto minStr = format_number(static_cast<long long>(time[0])) + u8"µs";
	auto maxStr = format_number(static_cast<long long>(time[2])) + u8"µs";
	auto avgStr = format_number(static_cast<long long>(avg));
	if (!avgDecStr.empty())
		avgStr += "." + avgDecStr;
	avgStr += u8"µs";
	if (!dry)
	{
		printElement(name, nameLen);
		cout << " | ";
		printElement(minStr, minLen, true);
		cout << " | ";
		printElement(maxStr, maxLen, true);
		cout << " | ";
		printElement(avgStr, avgLen, true);
		cout << endl;
	}
	else
	{
		minLen = max(minLen, static_cast<int>(minStr.length()));
		maxLen = max(maxLen, static_cast<int>(maxStr.length()));
		avgLen = max(avgLen, static_cast<int>(avgStr.length()));
		nameLen = max(nameLen, static_cast<int>(name.length()));
	}
}

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	setvbuf(stdout, nullptr, _IOFBF, 1000);
	cout.imbue(locale(""));

	auto bubble100 = time(BubbleSort, 100);
	auto bubble1000 = time(BubbleSort, 1000);
	auto bubble10000 = time(BubbleSort, 10000);

	auto insertion100 = time(InsertionSort, 100);
	auto insertion1000 = time(InsertionSort, 1000);
	auto insertion10000 = time(InsertionSort, 10000);

	auto selection100 = time(SelectionSort, 100);
	auto selection1000 = time(SelectionSort, 1000);
	auto selection10000 = time(SelectionSort, 10000);

	auto merge100 = time(MergeSort, 100);
	auto merge1000 = time(MergeSort, 1000);
	auto merge10000 = time(MergeSort, 10000);

	auto quick100 = time(QuickSort, 100);
	auto quick1000 = time(QuickSort, 1000);
	auto quick10000 = time(QuickSort, 10000);

	auto cocktail100 = time(CocktailSort, 100);
	auto cocktail1000 = time(CocktailSort, 1000);
	auto cocktail10000 = time(CocktailSort, 10000);

	auto heap100 = time(HeapSort, 100);
	auto heap1000 = time(HeapSort, 1000);
	auto heap10000 = time(HeapSort, 10000);

	cout << endl;
	cout << endl;
	cout << endl;

	formatTime("Bubble Sort 10000", bubble10000, true);
	formatTime("Insertion Sort 10000", insertion10000, true);
	formatTime("Selection Sort 10000", selection10000, true);
	formatTime("Merge Sort 10000", merge10000, true);
	formatTime("Quick Sort 10000", quick10000, true);
	formatTime("Cocktail Sort 10000", cocktail10000, true);
	formatTime("Heap Sort 10000", heap10000, true);

	printElement("Type", nameLen);
	cout << " | ";
	printElement("Min", minLen);
	cout << "| ";
	printElement("Max", maxLen);
	cout << "| ";
	printElement("Avg", avgLen);
	cout << endl;

	formatTime("Bubble Sort 100", bubble100);
	formatTime("Bubble Sort 1000", bubble1000);
	formatTime("Bubble Sort 10000", bubble10000);

	formatTime("Insertion Sort 100", insertion100);
	formatTime("Insertion Sort 1000", insertion1000);
	formatTime("Insertion Sort 10000", insertion10000);

	formatTime("Selection Sort 100", selection100);
	formatTime("Selection Sort 1000", selection1000);
	formatTime("Selection Sort 10000", selection10000);

	formatTime("Merge Sort 100", merge100);
	formatTime("Merge Sort 1000", merge1000);
	formatTime("Merge Sort 10000", merge10000);

	formatTime("Quick Sort 100", quick100);
	formatTime("Quick Sort 1000", quick1000);
	formatTime("Quick Sort 10000", quick10000);

	formatTime("Cocktail Sort 100", cocktail100);
	formatTime("Cocktail Sort 1000", cocktail1000);
	formatTime("Cocktail Sort 10000", cocktail10000);

	formatTime("Heap Sort 100", heap100);
	formatTime("Heap Sort 1000", heap1000);
	formatTime("Heap Sort 10000", heap10000);

	return 0;
}