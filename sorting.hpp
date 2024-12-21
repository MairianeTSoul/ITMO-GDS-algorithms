#include <algorithm>
#include <iterator>
#include <functional>
#include <utility>

int treshold = 32;

using namespace std;

template<typename T, typename Compare>
void insertion_sort(T* first, T* last, Compare comp) {
    for (T* i = first + 1; i < last; ++i)
    {
        T value = move(*i);
        T* temp = i;
        while (temp > first && comp(value, *(temp - 1)))
        {
            *temp = move(*(temp - 1));
            --temp;
        }
        *temp = move(value);
    }
}

template <typename T, typename Compare>
T& median_of_three(T* first, T* mid, T* last, Compare comp) {
    if (comp(*mid, *first))
    {
        T temp = move(*mid);
        *mid = move(*first);
        *first = move(temp);
    }

    if (comp(*last, *first))
    {
        T temp = move(*last);
        *last = move(*first);
        *first = move(temp);
    }

    if (comp(*last, *mid))
    {
        T temp = move(*last);
        *last = move(*mid);
        *mid = move(temp);
    }

    return *mid;
}

template<typename T, typename Compare>
double violation_coeff(T* first, T* last, Compare comp)
{
    int violations = 0;

    for (T* it = first + 1; it < last; ++it) {
        if (comp(*(it - 1), *it)) {
            ++violations;
        }
    }

    return static_cast<double>(violations) / (distance(first, last) - 1);
}


template<typename T, typename Compare>
void quick_sort(T* first, T* last, Compare comp) {
    while (distance(first, last) > 1)
    {
        int size = distance(first, last);

        T reference_element = median_of_three(first, first + (size / 2), last - 1, comp);

        T* left = first;
        T* right = last - 1;

        while (true)
        {
            while (comp(*left, reference_element)) {
                ++left;
            }

            while (comp(reference_element, *right)) {
                --right;
            }

            if (left >= right) {
                break;
            }

            T temp = move(*left);
            *left = move(*right);
            *right = move(temp);

            ++left;
            --right;
        }

        T* mid = left;

        if (distance(first, mid) < distance(mid, last))
        {
            quick_sort(first, mid, comp);
            first = mid;
        }
        else
        {
            quick_sort(mid, last, comp);
            last = mid;
        }
    }
}

template<typename T, typename Compare>
void combo_sort(T* first, T* last, Compare comp) {
    while (distance(first, last) > 1)
    {
        int size = distance(first, last);

        if (size <= treshold)
        {
            if (violation_coeff(first, last, comp) > 0.1)
            {
                insertion_sort(first, last, comp);
                return;
            }
        }

        T reference_element = median_of_three(first, first + (size / 2), last - 1, comp);

        T* left = first;
        T* right = last - 1;

        while (true)
        {
            while (comp(*left, reference_element)) {
                ++left;
            }

            while (comp(reference_element, *right)) {
                --right;
            }

            if (left >= right) {
                break;
            }

            T temp = move(*left);
            *left = move(*right);
            *right = move(temp);

            ++left;
            --right;
        }

        T* mid = left;

        if (distance(first, mid) < distance(mid, last))
        {
            combo_sort(first, mid, comp);
            first = mid;
        }
        else
        {
            combo_sort(mid, last, comp);
            last = mid;
        }
    }
}
