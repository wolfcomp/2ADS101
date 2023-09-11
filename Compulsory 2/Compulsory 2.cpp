#include <algorithm>
#include <iostream>
#include <chrono>
#include <random>

#include "BubbleSort.h"
#include "Console.h"
#include "IntroSort.h"
#include "MergeSort.h"

auto rnd = std::default_random_engine{ std::random_device{}() };

std::vector<int> generate_random_vector(const int size)
{
    std::vector<int> vec;
    vec.reserve(size);
    for (int i = 0; i < size; i++)
    {
        vec.push_back(i + 1);
    }
    std::shuffle(vec.begin(), vec.end(), rnd);
    return vec;
}

/**
 * \brief Measure the time of the sorting
 * \param sort The sorting function to use
 * \param size The array size to use
 * \param alg The name of the algorithm used
 * \return The time it took to sort the list
 */
double time(void(sort)(std::vector<int>& arr), const int size, const std::string& alg)
{
    std::cout << "Creating random array of size: " << size << std::endl;
    auto vec = generate_random_vector(size);
    std::cout << "Sorting array of size: " << size << " with algorithm " << alg << std::endl;
    const std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    sort(vec);
    return static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - t1).count());
}

/**
 * \brief Calculates the standard deviation and finds max and min time
 * \param times The list of times that should be used in the calculation
 * \return List of size 3 with min,max,sd
 */
std::vector<double> time_min_max_sd(std::vector<double> times)
{
    std::vector<double> ret;
    double sum = 0, sd = 0;
    for (const auto& time : times)
    {
        sum += time;
    }
    const double mean = sum / static_cast<double>(times.size());
    for (const auto& time : times)
    {
        sd += pow(time - mean, 2);
    }
    return { *std::min_element(times.begin(), times.end()), *std::max_element(times.begin(), times.end()), sqrt(sd / static_cast<double>(times.size())) };
}

/**
 * \brief Measure the time of the sorting with 10 iterations
 * \param sort The sorting function to use
 * \param size The array size to use
 * \param alg The name of the algorithm used
 * \return List of size 3 with values in order as min,max,sd
 */
std::vector<double> time_multiple(void(sort)(std::vector<int>& arr), const int size, const std::string& alg)
{
    std::vector<double> times;
    times.reserve(10);
    for (int i = 0; i < 10; i++)
    {
        times.push_back(time(sort, size, alg));
    }
    return time_min_max_sd(times);
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    auto _ = setvbuf(stdout, nullptr, _IOFBF, 1024);

    const auto bubbleSortTimes10 = time_multiple(BubbleSort::sort, 10, "BubbleSort");
    const auto introSortTimes10 = time_multiple(IntroSort::sort, 10, "IntroSort");
    const auto mergeSortTimes10 = time_multiple(MergeSort::sort, 10, "MergeSort");

    const auto bubbleSortTimes100 = time_multiple(BubbleSort::sort, 100, "BubbleSort");
    const auto introSortTimes100 = time_multiple(IntroSort::sort, 100, "IntroSort");
    const auto mergeSortTimes100 = time_multiple(MergeSort::sort, 100, "MergeSort");

    const auto bubbleSortTimes1000 = time_multiple(BubbleSort::sort, 1000, "BubbleSort");
    const auto introSortTimes1000 = time_multiple(IntroSort::sort, 1000, "IntroSort");
    const auto mergeSortTimes1000 = time_multiple(MergeSort::sort, 1000, "MergeSort");

    const auto bubbleSortTimes10000 = time_multiple(BubbleSort::sort, 10000, "BubbleSort");
    const auto introSortTimes10000 = time_multiple(IntroSort::sort, 10000, "IntroSort");
    const auto mergeSortTimes10000 = time_multiple(MergeSort::sort, 10000, "MergeSort");

    console::TimeFormat::print_time("Bubble Sort 10000", bubbleSortTimes10000, true);
    console::TimeFormat::print_time("Merge Sort 10000", mergeSortTimes10000, true);
    console::TimeFormat::print_time("Intro Sort 10000", introSortTimes10000, true);

    for (int i = 0; i < 50; ++i)
    {
        std::cout << std::endl;
    }

    console::TimeFormat::print_header();
    console::TimeFormat::print_separator();
    console::TimeFormat::print_time("Bubble Sort 10", bubbleSortTimes10);
    console::TimeFormat::print_time("Bubble Sort 100", bubbleSortTimes100);
    console::TimeFormat::print_time("Bubble Sort 1000", bubbleSortTimes1000);
    console::TimeFormat::print_time("Bubble Sort 10000", bubbleSortTimes10000);
    console::TimeFormat::print_separator();
    console::TimeFormat::print_time("Merge Sort 10", mergeSortTimes10);
    console::TimeFormat::print_time("Merge Sort 100", mergeSortTimes100);
    console::TimeFormat::print_time("Merge Sort 1000", mergeSortTimes1000);
    console::TimeFormat::print_time("Merge Sort 10000", mergeSortTimes10000);
    console::TimeFormat::print_separator();
    console::TimeFormat::print_time("Intro Sort 10", introSortTimes10);
    console::TimeFormat::print_time("Intro Sort 100", introSortTimes100);
    console::TimeFormat::print_time("Intro Sort 1000", introSortTimes1000);
    console::TimeFormat::print_time("Intro Sort 10000", introSortTimes10000);
}
