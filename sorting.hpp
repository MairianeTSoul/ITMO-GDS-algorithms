#include <algorithm>
#include <iterator>
#include <functional>
#include <utility>

template<typename T, typename Compare>
void insertion_sort(T* first, T* last, Compare comp) {
    for (T* i = first + 1; i != last; ++i) {
        T* j = i;
        while (j > first && comp(*j, *(j - 1))) {
            std::swap(*j, *(j - 1));
            --j;
        }
    }
}

template <typename T, typename Compare>
T& median_of_three(T* first, T* mid, T* last, Compare comp) {
    if (comp(*mid, *first)) std::swap(*mid, *first);
    if (comp(*(last - 1), *first)) std::swap(*(last - 1), *first);
    if (comp(*(last - 1), *mid)) std::swap(*(last - 1), *mid);
    return *mid;
}

template<typename T, typename Compare>
T* partition(T* first, T* last, Compare comp) {
    T* mid = first + (last - first) / 2;
    median_of_three(first, mid, last, comp);
    std::swap(*(last - 1), *mid);

    T* i = first;
    for (T* j = first; j < last - 1; ++j) {
        if (comp(*j, *(last - 1))) {
            std::swap(*i, *j);
            ++i;
        }
    }

    std::swap(*(last - 1), *i);
    return i;
}

template<typename T, typename Compare>
void quick_sort(T* first, T* last, Compare comp) {
    while (first < last) {
        T* pivot = partition(first, last, comp);

        if (pivot - first < last - pivot) {
            combo_sort(first, pivot, comp);
            first = pivot + 1;
        } else {
            combo_sort(pivot + 1, last, comp);
            last = pivot - 1;
        }
    }
}

template<typename T, typename Compare>
void combo_sort(T* first, T* last, Compare comp) {
    const int threshold = 12;

    while (first < last) {
        if (last - first <= threshold) {
            insertion_sort(first, last, comp);
            break;
        }
        T* pivot = partition(first, last, comp);

        if (pivot - first < last - pivot) {
            combo_sort(first, pivot, comp);
            first = pivot + 1;
        } else {
            combo_sort(pivot + 1, last, comp);
            last = pivot - 1;
        }
    }
}

