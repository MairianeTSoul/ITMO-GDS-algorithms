
// Array Private
template<typename T>
void Array<T>::clear() {
    for (int i = 0; i < currentSize; ++i) {
        data[i].~T();
    }
    currentSize = 0;
}

template<typename T>
void Array<T>::reallocate() {
    capacity = static_cast<int>(capacity * 1.6);
    T* newData = static_cast<T*>(malloc(sizeof(T) * capacity));
    for (int i = 0; i < currentSize; ++i) {
        new (&newData[i]) T(std::move(data[i]));
        data[i].~T();
    }
    free(data);
    data = newData;
}

// Array Public

template<typename T>
Array<T>::Array(int cap) : capacity(cap), currentSize(0) {
    data = static_cast<T*>(malloc(sizeof(T) * capacity));
}

template<typename T>
Array<T>::~Array() {
    clear();
    free(data);
}

template<typename T>
Array<T>::Array(const Array& other) : capacity(other.capacity), currentSize(other.currentSize) {
    data = static_cast<T*>(malloc(sizeof(T) * capacity));
    for (int i = 0; i < currentSize; ++i) {
        new (&data[i]) T(other.data[i]);
    }
}

template<typename T>
Array<T>& Array<T>::operator=(const Array& other) {
    if (this != &other) {
        clear();
        free(data);
        capacity = other.capacity;
        currentSize = other.currentSize;
        data = static_cast<T*>(malloc(sizeof(T) * capacity));
        for (int i = 0; i < currentSize; ++i) {
            new (&data[i]) T(other.data[i]);
        }
    }
    return *this;
}

template<typename T>
Array<T>::Array(Array&& other) noexcept : data(other.data), capacity(other.capacity), currentSize(other.currentSize)
{
    other.data = nullptr;
    other.capacity = 0;
    other.currentSize = 0;
}

template<typename T>
Array<T>& Array<T>::operator=(Array&& other) noexcept {
    if (this != &other)
    {
        clear();
        free(data);
        data = other.data;
        capacity = other.capacity;
        currentSize = other.currentSize;
        other.data = nullptr;
        other.capacity = 0;
        other.currentSize = 0;
    }
    return *this;
}

template<typename T>
int Array<T>::insert(const T& value) {
    if (currentSize == capacity) {
        reallocate();
    }
    new (&data[currentSize]) T(value);
    return currentSize++;
}

template<typename T>
int Array<T>::insert(int index, const T& value) {
    if (index < 0 || index > currentSize) {
        throw std::out_of_range("Index out of range");
    }
    if (currentSize == capacity) {
        reallocate();
    }
    for (int i = currentSize; i > index; --i) {
        new (&data[i]) T(std::move(data[i - 1]));
        data[i - 1].~T();
    }
    new (&data[index]) T(value);
    ++currentSize;
    return index;
}

template<typename T>
void Array<T>::remove(int index) {
    if (index < 0 || index >= currentSize) {
        throw std::out_of_range("Index out of range");
    }
    for (int i = index; i < currentSize - 1; ++i) {
        new (&data[i]) T(std::move(data[i + 1]));
        data[i + 1].~T();
    }
    --currentSize;
}

template<typename T>
const T& Array<T>::operator[](int index) const {
    if (index < 0 || index >= currentSize) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

template<typename T>
T& Array<T>::operator[](int index) {
    if (index < 0 || index >= currentSize) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

template<typename T>
int Array<T>::size() const {
    return currentSize;
}

// Iterator

template<typename T>
Array<T>::Iterator::Iterator(T* p, Array<T>& arr) : ptr(p), array(arr) {}

template<typename T>
T& Array<T>::Iterator::get() const { return *ptr; }

template<typename T>
void Array<T>::Iterator::set(const T& value) { *ptr = value; }

template<typename T>
void Array<T>::Iterator::next() { if (hasNext()) ++ptr; }

template<typename T>
bool Array<T>::Iterator::hasNext() const { return ptr < (array.data + array.currentSize); }

template<typename T>
typename Array<T>::Iterator Array<T>::iterator() {
    return Iterator(data, *this);
}

// ConstIterator

template<typename T>
Array<T>::ConstIterator::ConstIterator(const T* p, const Array<T>& arr) : ptr(p), array(arr) {}

template<typename T>
const T& Array<T>::ConstIterator::get() const { return *ptr; }

template<typename T>
void Array<T>::ConstIterator::next() { if (hasNext()) ++ptr; }

template<typename T>
bool Array<T>::ConstIterator::hasNext() const { return ptr < (array.data + array.currentSize); }

template<typename T>
typename Array<T>::ConstIterator Array<T>::const_iterator() const {
    return ConstIterator(data, *this);
}

// ReverseIterator

template<typename T>
Array<T>::ReverseIterator::ReverseIterator(T* p, Array<T>& arr) : ptr(p), array(arr) {}

template<typename T>
T& Array<T>::ReverseIterator::get() const { return *ptr; }

template<typename T>
void Array<T>::ReverseIterator::set(const T& value) { *ptr = value; }

template<typename T>
void Array<T>::ReverseIterator::next() { if (hasNext()) --ptr; }

template<typename T>
bool Array<T>::ReverseIterator::hasNext() const { return ptr >= (array.data); }

template<typename T>
typename Array<T>::ReverseIterator Array<T>::reverseIterator() {
    return ReverseIterator(data + currentSize - 1, *this);
}

// ConstReverseIterator implementation

template<typename T>
Array<T>::ConstReverseIterator::ConstReverseIterator(const T* p, const Array<T>& arr) : ptr(p), array(arr) {}

template<typename T>
const T& Array<T>::ConstReverseIterator::get() const { return *ptr; }

template<typename T>
void Array<T>::ConstReverseIterator::next() { if (hasNext()) --ptr; }

template<typename T>
bool Array<T>::ConstReverseIterator::hasNext() const { return ptr >= (array.data); }

template<typename T>
typename Array<T>::ConstReverseIterator Array<T>::const_reverseIterator() const {
    return ConstReverseIterator(data + currentSize - 1, *this);
}

