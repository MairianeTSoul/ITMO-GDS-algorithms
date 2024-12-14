#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"
#include "sorting.hpp"
#include <random>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

void generateRandomArray(int* arr, int size, int seed = 30) {
    mt19937 gen(seed);
    uniform_int_distribution<> dist(1, 1000000);

    for (int i = 0; i < size; ++i) {
        arr[i] = dist(gen);
    }
}

TEST(InsertionSortTest, SortSmallArray) {
    int arr[] = {5, 3, 4, 1, 2};
    insertion_sort(arr, arr + 5, [](int a, int b) { return a < b; });
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
    EXPECT_EQ(arr[3], 4);
    EXPECT_EQ(arr[4], 5);
}

TEST(InsertionSortTest, EmptyArray) {
    int arr[] = {};
    insertion_sort(arr, arr + 0, [](int a, int b) { return a < b; });
    EXPECT_EQ(arr[0], 0);
}

TEST(InsertionSortTest, SingleElementArray) {
    int arr[] = {10};
    insertion_sort(arr, arr + 1, [](int a, int b) { return a < b; });
    EXPECT_EQ(arr[0], 10);
}

TEST(InsertionSortTest, ArrayWithIdenticalElements) {
    int arr[] = {7, 7, 7, 7};
    insertion_sort(arr, arr + 4, [](int a, int b) { return a < b; });
    EXPECT_EQ(arr[0], 7);
    EXPECT_EQ(arr[1], 7);
    EXPECT_EQ(arr[2], 7);
    EXPECT_EQ(arr[3], 7);
}

TEST(MedianOfThreeTest, CorrectMedian) {
    int arr[] = {4, 5, 7, 10, 1, 2, -1};

    int *first = arr;
    int *last = arr + 7;
    int *mid = first + (last - first) / 2;

    auto median = median_of_three(first, mid, last, [](int a, int b) { return a < b; });
    EXPECT_EQ(median, 4);
}


TEST(QuickSortTest, SortArray) {
    int arr[] = {10, 5, 3, 7, 6, 1, 9, 8, 4, 2};
    sort(arr, arr + 10, [](int a, int b) { return a < b; });
    for (int i = 0; i < 9; ++i) {
        EXPECT_LE(arr[i], arr[i + 1]);
    }
}

TEST(QuickSortTest, SortWithDoubleAndNegativeNumbers) {
    double arr[] = {1.4, -2.1, 3.4, -4.3, 5.33, 0.1};
    sort(arr, arr + 6, [](double a, double b) { return a < b; });
    EXPECT_EQ(arr[0], -4.3);
    EXPECT_EQ(arr[1], -2.1);
    EXPECT_EQ(arr[2], 0.1);
    EXPECT_EQ(arr[3], 1.4);
    EXPECT_EQ(arr[4], 3.4);
    EXPECT_EQ(arr[5], 5.33);
}

TEST(QuickSortTest, SortLargeArray) {
    const int size = 10000000;
    int arr[size];
    for (int i = 0; i < size; ++i) {
        arr[i] = size - i;
    }
    sort(arr, arr + size, [](int a, int b) { return a < b; });
    for (int i = 0; i < size - 1; ++i) {
        EXPECT_LE(arr[i], arr[i + 1]);
    }
}

TEST(SortingTest, SortStringsAlphabetically) {
    std::string strings[] = {"banana", "apple", "cherry", "date"};
    std::string expected[] = {"apple", "banana", "cherry", "date"};
    int size = sizeof(strings) / sizeof(strings[0]);

    sort(strings, strings + size, [](const std::string &a, const std::string &b) {
        return a < b;
    });

    for (int i = 0; i < size; ++i) {
        EXPECT_EQ(strings[i], expected[i]);
    }
}

TEST(SortingTest, SortStringsByLength) {
    std::string strings[] = {"kiwi", "pineapple", "pear", "apple"};
    std::string expected[] = {"kiwi", "pear", "apple", "pineapple"};
    int size = sizeof(strings) / sizeof(strings[0]);

    sort(strings, strings + size, [](const std::string &a, const std::string &b) {
        return a.size() < b.size();
    });

    for (int i = 0; i < size; ++i) {
        EXPECT_EQ(strings[i], expected[i]);
    }
}

TEST(SortingTest, SortStringsReverseAlphabetical) {
    std::string strings[] = {"banana", "apple", "cherry", "date"};
    std::string expected[] = {"date", "cherry", "banana", "apple"};
    int size = sizeof(strings) / sizeof(strings[0]);

    sort(strings, strings + size, [](const std::string &a, const std::string &b) {
        return a > b;
    });

    for (int i = 0; i < size; ++i) {
        EXPECT_EQ(strings[i], expected[i]);
    }
}

void runBenchmark(const std::string& sortName, std::function<void(int*, int*)> sortFunc) {
    ankerl::nanobench::Bench bench;
    std::cout << sortName << std::endl;
    bench.title(sortName).relative(true);

    std::vector<int> sizes;

    for (int i = 10000; i < 100000; i += 10000) sizes.push_back(i);
    for (int i = 100000; i <= 1000000; i += 100000) sizes.push_back(i);

    for (int size : sizes) {
        std::vector<int> arr(size);

        generateRandomArray(arr.data(), size);

        bench.run(std::to_string(size) + " elements", [&] {
            sortFunc(arr.data(), arr.data() + size);
            ankerl::nanobench::doNotOptimizeAway(arr);
        });
    }
}

int main(int argc, char** argv) {

    ::testing::InitGoogleTest(&argc, argv);

    cout << "Test 1" << std::endl;

    runBenchmark("sort", [](int* begin, int* end) {
        sort(begin, end, [](int a, int b) { return a < b; });
    });

    runBenchmark("quick sort", [](int* begin, int* end) {
        quick_sort(begin, end, [](int a, int b) { return a < b; });
    });

    runBenchmark("insertion sort", [](int* begin, int* end) {
        insertion_sort(begin, end, [](int a, int b) { return a < b; });
    });

        cout << "Test 2" << std::endl;

        std::vector<int> testSizes;

        for (int i = 5; i <= 100; i += 5) testSizes.push_back(i);

        for (int size: testSizes) {
            std::vector<int> arr(size);
            generateRandomArray(arr.data(), size);

            auto start = std::chrono::high_resolution_clock::now();
            insertion_sort(arr.data(), arr.data() + size, [](int a, int b) { return a < b; });
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> insertionSortTime = end - start;

            generateRandomArray(arr.data(), size);

            start = std::chrono::high_resolution_clock::now();
            quick_sort(arr.data(), arr.data() + size, [](int a, int b) { return a < b; });
            end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> quickSortTime = end - start;

            generateRandomArray(arr.data(), size);

            start = std::chrono::high_resolution_clock::now();
            sort(arr.data(), arr.data() + size, [](int a, int b) { return a < b; });
            end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> sortTime = end - start;

            std::cout << "Size: " << size
                      << "  insertion sort: " << insertionSortTime.count() * 1e9
                      << "ns, quick sort: " << quickSortTime.count() * 1e9
                      << "ns, sort: " << sortTime.count() * 1e9 << "ns min: ";
            if (insertionSortTime <= quickSortTime && insertionSortTime <= sortTime) {
                std::cout << "insertion sort\n";
            } else if (quickSortTime <= insertionSortTime && quickSortTime <= sortTime) {
                std::cout << "quick sort\n";
            } else {
                std::cout << "sort\n";
            }
        }

        cout << "Test 3" << std::endl;

        return RUN_ALL_TESTS();
    }