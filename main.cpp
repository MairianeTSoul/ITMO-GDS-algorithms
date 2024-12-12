#include "Array.h"
#include <iostream>

int main() {
    Array<int> arr;

    arr.insert(10);
    arr.insert(20);
    arr.insert(30);

    for (auto it = arr.iterator(); it.hasNext(); it.next()) {
        std::cout << "Before change: " << it.get() << std::endl;
        it.get() += 5;
    }

    for (auto cit = arr.const_iterator(); cit.hasNext(); cit.next()) {
        std::cout << "ConstIterator: " << cit.get() << std::endl;
        // cit.get() += 5; // Cannot assign to return value because function 'get' returns a const value
    }
    return 0;
}