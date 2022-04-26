#pragma once

#include <algorithm>
#include <cassert>
#include <cstring>
#include <ostream>

//#include <iostream>

constexpr size_t START_SIZE = 2;

enum retStatus {
    OK,
    ERROR
};

template<class T>
class Vector {
public:
    Vector(T const &first_elem = T());
    Vector(Vector const &other);
    ~Vector();
    Vector &operator=(Vector const &other);
    retStatus addElement(T const &elem);
    retStatus insertElementSorted(T const &elem);
    retStatus deleteElement(size_t i);
    T &operator[](size_t i);
    T const &operator[](size_t i) const;
    void sort();
    size_t getSize() const;
    size_t getMaxSize() const;
    size_t find(T const &elem) const;
    retStatus clear();
    void swap(size_t i, size_t j);
    retStatus resize(size_t new_max_size);
protected:
    size_t max_size;
    size_t size;
    T *data;
private:
    void freeVector();
    retStatus binSearch(T const &elem, size_t &left, size_t &right) const;
};

template<class T>
Vector<T>::Vector(T const &first_elem)
    : max_size(START_SIZE)
    , size(0)
    , data(new(std::nothrow) T[START_SIZE]) {
    assert(data);
    //std::cout << "Out of if:" << std::endl;
    //if (first_elem) {
        //std::cout << "Inside of if:" << std::endl;
        assert(addElement(first_elem) == OK);
    //}
}

template<class T>
Vector<T>::Vector(Vector const &other) : Vector() {
    *this = other;
}

template<class T>
Vector<T>::~Vector() {
    freeVector();
    max_size = 0;
    size = 0;
    data = nullptr;
}

template<class T>
Vector<T> &Vector<T>::operator=(Vector const &other) {
    clear();
    for (size = 0; size < other.size;) {
        assert(addElement(other[size]) == OK);
    }
    return *this;
}

template<class T>
retStatus Vector<T>::addElement(T const &elem) {
    if (size == max_size) {
        if (resize(max_size * 2) == ERROR) {
            return ERROR;
        }
    }
    data[size] = elem;
    ++size;
    return OK;
}

template<class T>
retStatus Vector<T>::insertElementSorted(T const &elem) {
    if (addElement(elem) == ERROR) {
        return ERROR;
    }
    if (size == 1) {
        return OK;
    }
    for (size_t inserted = size - 1
         ; inserted != 0 && data[inserted] < data[inserted - 1]
         ; --inserted) {
        swap(inserted - 1, inserted);
    }
    return OK;
}

template<class T>
retStatus Vector<T>::deleteElement(size_t i) {
    if (i >= size) {
        return ERROR;
    }
    for (; i + 1 < size; ++i) {
        swap(i, i + 1);
    }
    data[size - 1].~T();
    if (size == max_size / 4) {
        if (resize(max_size / 2) == ERROR) {
            return ERROR;
        }
    }
    --size;
    return OK;
}

template<class T>
T &Vector<T>::operator[](size_t i) {
    assert(i < size);

    return data[i];
}

template<class T>
T const &Vector<T>::operator[](size_t i) const {
    assert(i < size);

    return data[i];
}

template<class T>
void Vector<T>::sort() {
    std::sort(data, data + size);
}

template<class T>
size_t Vector<T>::getSize() const {
    return size;
}

template<class T>
size_t Vector<T>::getMaxSize() const {
    return max_size;
}

template<class T>
size_t Vector<T>::find(T const &elem) const {
    if (!size) {
        return static_cast<size_t>(-1);
    }
    size_t left = 0;
    size_t right = size;
    if (binSearch(elem, left, right) == ERROR) {
        return static_cast<size_t>(-1);
    }
    //std::cout << "bbbbbb " << left << std::endl;
    return (left == right) ? left : static_cast<size_t>(-1);
}

template<class T>
retStatus Vector<T>::clear() {
    freeVector();
    max_size = START_SIZE;
    size = 0;
    data = new(std::nothrow) T[START_SIZE];
    if (!data) {
        return ERROR;
    }
    return OK;
}

template<class T>
void Vector<T>::swap(size_t i, size_t j) {
    T tmp = data[j];
    data[j] = data[i];
    data[i] = tmp;
}

template<class T>
retStatus Vector<T>::resize(size_t new_max_size) {
    if (max_size == new_max_size) {
        return OK;
    }
    size_t new_size = (size < new_max_size) ? size : new_max_size;
    if (new_max_size < START_SIZE) {
        new_max_size = START_SIZE;
    }
    T *newData = new(std::nothrow) T[new_max_size];
    if (!newData) {
        return ERROR;
    }
    for (max_size = 0; max_size < new_size; ++max_size) {
        newData[max_size] = data[max_size];
    }
    delete[] data;
    data = newData;
    if (size > new_size) {
        size = new_size;
    }
    max_size = new_max_size;
    return OK;
}

template<class T>
void Vector<T>::freeVector() {
    for (--size; size + 1 != 0; --size) {
        data[size].~T();
    }
    delete[] data;
}

template<class T>
retStatus Vector<T>::binSearch(T const &elem
                               , size_t &left
                               , size_t &right) const {
    //std::cout << "{{{\nStarting binSearch at [" << left << "; " << right << "]:\n";
    if (!size) {
        return ERROR;
    }
    size_t mid = (left + right) / 2;
    //std::cout << "Size is OK, mid set at " << mid << ".\n";
    //std::cout << "Finding " << elem << " in " << *this << std::endl;
    while (left < mid && mid < right) {
        (mid >= size) ? right = mid
                      : ((data[mid] < elem) ? left = mid : right = mid);
        mid = (left + right) / 2;
        //std::cout << "Updated left(" << left << "), mid(" << mid << "), right(" << right << ").\n";
    }
    //std::cout << "Checking: " << (data[left] < elem) << ", " << (elem < data[left]) << std::endl;
    if (!(data[left] < elem || elem < data[left])) {
        right = left;
        //std::cout << "Set right(" << right << ").\n";
        return OK;
    }
    if (right < size) {
        //std::cout << "Checking: " << (data[right] < elem) << ", " << (elem < data[right]) << std::endl;
        if (!(data[right] < elem || elem < data[right])) {
            left = right;
            //std::cout << "Set left(" << left << ").\n";
            return OK;
        }
    }
    //std::cout << "Finishing binSearch with left = " << left << ", right = " << right << ".\n}}}"<< std::endl;
    return OK;
}

template<class T>
std::ostream& operator<<(std::ostream &out, Vector<T> const &vector) {
    size_t size = vector.getSize();
    out << '[' << size << " out of " << vector.getMaxSize() << "]:";
    for (size_t i = 0; i < size; ++i) {
        out << " \"" << vector[i] << '\"';
    }
    out << '.';
    return out;
}
