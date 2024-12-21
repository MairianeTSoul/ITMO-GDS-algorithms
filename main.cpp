#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"
#include "sorting.hpp"
#include <random>
#include <vector>
#include <fstream>
#include <filesystem>

#include <gtest/gtest.h>

using namespace std;
using namespace chrono;

void save_results_to_csv(const vector<int>& sizes, const vector<double>& rev_is, const vector<double>& rev_qs, const vector<double>& rev_cqs)
{
    filesystem::create_directory("result");
    string file_path = "result/data_" + std::to_string(treshold) + "_" +
                       std::to_string(static_cast<int>(rev_qs[rev_cqs.size() - 1] * 1000000)) + "_.csv";

    ofstream file(file_path);

    if (!file.is_open())
    {
        cerr << "Cann't open Data.csv\n";
        return;
    }

    file << "size,insertion_sort,quick_sort,combo_sort\n";

    for (size_t i = 0; i < sizes.size(); ++i) {
        file << sizes[i] << "," << rev_is[i] * 1e9 << "," << rev_qs[i] * 1e9 << "," << rev_cqs[i] * 1e9 << "\n";
    }

    file.close();
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
    EXPECT_EQ(arr, arr + 0);
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

TEST(QuickSortTest, SortArray) {
    int arr[] = {10, 5, 3, 7, 6, 1, 9, 8, 4, 2};
    combo_sort(arr, arr + 10, [](int a, int b) { return a < b; });
    for (int i = 0; i < 9; ++i) {
        EXPECT_LE(arr[i], arr[i + 1]);
    }
}

TEST(QuickSortTest, SortWithDoubleAndNegativeNumbers) {
    double arr[] = {1.4, -2.1, 3.4, -4.3, 5.33, 0.1};
    combo_sort(arr, arr + 6, [](double a, double b) { return a < b; });
    EXPECT_EQ(arr[0], -4.3);
    EXPECT_EQ(arr[1], -2.1);
    EXPECT_EQ(arr[2], 0.1);
    EXPECT_EQ(arr[3], 1.4);
    EXPECT_EQ(arr[4], 3.4);
    EXPECT_EQ(arr[5], 5.33);
}

TEST(QuickSortTest, SortLargeArray) {
    const int size = 100000;
    int arr[size];
    for (int i = 0; i < size; ++i) {
        arr[i] = size - i;
    }
    combo_sort(arr, arr + size, [](int a, int b) { return a < b; });
    for (int i = 0; i < size - 1; ++i) {
        EXPECT_LE(arr[i], arr[i + 1]);
    }
}

TEST(SortingTest, SortStringsAlphabetically) {
    string strings[] = {"banana", "apple", "cherry", "date"};
    string expected[] = {"apple", "banana", "cherry", "date"};
    int size = sizeof(strings) / sizeof(strings[0]);

    combo_sort(strings, strings + size, [](const string &a, const string &b) {
        return a < b;
    });

    for (int i = 0; i < size; ++i) {
        EXPECT_EQ(strings[i], expected[i]);
    }
}

TEST(SortingTest, SortStringsByLength) {
    string strings[] = {"kiwi", "pineapple", "pear", "apple"};
    string expected[] = {"kiwi", "pear", "apple", "pineapple"};
    int size = sizeof(strings) / sizeof(strings[0]);

    combo_sort(strings, strings + size, [](const string &a, const string &b) {
        return a.size() < b.size();
    });

    for (int i = 0; i < size; ++i) {
        EXPECT_EQ(strings[i], expected[i]);
    }
}

TEST(SortingTest, SortStringsReverseAlphabetical) {
    string strings[] = {"banana", "apple", "cherry", "date"};
    string expected[] = {"date", "cherry", "banana", "apple"};
    int size = sizeof(strings) / sizeof(strings[0]);

    combo_sort(strings, strings + size, [](const string &a, const string &b) {
        return a > b;
    });

    for (int i = 0; i < size; ++i) {
        EXPECT_EQ(strings[i], expected[i]);
    }
}

void insertion_sort_test(vector<int> arr)
{
    vector<int> arr_copy = arr;
    insertion_sort(arr_copy.data(), arr_copy.data() + arr_copy.size(), [](int a, int b) { return a < b; });
}

void quick_sort_test(vector<int> arr)
{
    vector<int> arr_copy = arr;
    quick_sort(arr.data(), arr.data() + arr.size(), [](int a, int b) { return a < b; });
}


void combo_sort_test(vector<int> arr)
{
    vector<int> arr_copy = arr;
    combo_sort(arr.data(), arr.data() + arr.size(), [](int a, int b) { return a < b; });
}


vector<int> generateArray(int size) {
    vector<int> arr(size);
    generate(arr.begin(), arr.end(), [n = size]() mutable { return --n; });
    return arr;
}

int main(int argc, char** argv) {
    for (treshold = 32; treshold <= 32; treshold++)
    {
        cout << treshold << endl;

        ::testing::InitGoogleTest(&argc, argv);

        vector<double> insertion_sort_times;
        vector<double> quick_sort_times;
        vector<double> combo_sort_times;

        vector<int> arr;

        int warmups = 1000, epochs = 10000;
        ankerl::nanobench::Bench bench;

        vector<int>testSizes;
        for (int i = 1; i <= 60; i++) {
            testSizes.push_back(i);
        }

        for (int size : testSizes) {
            cout << "Size: " << size << endl;
            arr = generateArray(size);

            bench = ankerl::nanobench::Bench().warmup(warmups).minEpochIterations(epochs);
            bench.run("Insertion sort", [&]() {insertion_sort_test(arr); });
            insertion_sort_times.push_back(bench.results().front().median(ankerl::nanobench::Result::Measure::elapsed));

            bench = ankerl::nanobench::Bench().warmup(warmups).minEpochIterations(epochs);
            bench.run("Default Quick sort", [&]() {quick_sort_test(arr); });
            quick_sort_times.push_back(bench.results().front().median(ankerl::nanobench::Result::Measure::elapsed));


            bench = ankerl::nanobench::Bench().warmup(warmups).minEpochIterations(epochs);
            bench.run("Quick sort", [&]() {combo_sort_test(arr); });
            combo_sort_times.push_back(bench.results().front().median(ankerl::nanobench::Result::Measure::elapsed));

            cout << "Size: " << size;

            auto start = high_resolution_clock::now();
            insertion_sort_test(arr);
            auto end = high_resolution_clock::now();
            duration<double> insertionSortTime = end - start;
            cout << "; Insertion Sort Time: " << duration_cast<nanoseconds>(insertionSortTime).count() << " ns";

            start = high_resolution_clock::now();
            quick_sort_test(arr);
            end = high_resolution_clock::now();
            duration<double> quickSortTime = end - start;
            cout << "; Quick Sort Time: " << duration_cast<nanoseconds>(quickSortTime).count() << " ns";

            start = high_resolution_clock::now();
            combo_sort_test(arr);
            end = high_resolution_clock::now();
            duration<double> sortTime = end - start;
            cout << "; Combo Sort Time: " << duration_cast<nanoseconds>(sortTime).count() << " ns" << endl;
        }

        save_results_to_csv(testSizes, insertion_sort_times, quick_sort_times, combo_sort_times);


    }

    cout << "Test 3" << endl;
    return RUN_ALL_TESTS();
}