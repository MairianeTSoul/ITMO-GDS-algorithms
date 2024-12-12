#pragma once
#include <stdexcept>
#include <utility>
#include <cstdlib>

template<typename T>
class Array {
private:
    T* data;
    int capacity;
    int currentSize;

    void clear();
    void reallocate();

public:
    Array(int cap = 8);
    ~Array();

    Array(const Array& other);
    Array& operator=(const Array& other);

    Array(Array&& other) noexcept;
    Array& operator=(Array&& other) noexcept;

    int insert(const T& value);
    int insert(int index, const T& value);

    void remove(int index);

    const T& operator[](int index) const;
    T& operator[](int index);

    int size() const;

    class Iterator {
    private:
        T* ptr;
        Array<T>& array;

    public:
        Iterator(T* p, Array<T>& arr);
        T& get() const;
        void set(const T& value);
        void next();
        bool hasNext() const;
    };

    class ConstIterator {
    private:
        const T* ptr;
        const Array<T>& array;

    public:
        ConstIterator(const T* p, const Array<T>& arr);
        const T& get() const;
        void next();
        bool hasNext() const;
    };

    class ReverseIterator {
    private:
        T* ptr;
        Array<T>& array;

    public:
        ReverseIterator(T* p, Array<T>& arr);
        T& get() const;
        void set(const T& value);
        void next();
        bool hasNext() const;
    };

    class ConstReverseIterator {
    private:
        const T* ptr;
        const Array<T>& array;

    public:
        ConstReverseIterator(const T* p, const Array<T>& arr);
        const T& get() const;
        void next();
        bool hasNext() const;
    };

    Iterator iterator();
    ConstIterator const_iterator() const;

    ReverseIterator reverseIterator();
    ConstReverseIterator const_reverseIterator() const;

};

#include "array.hpp"