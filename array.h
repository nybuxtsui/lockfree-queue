#ifndef _CARRAY_H_
#define _CARRAY_H_

#include <string.h>
#include <stdlib.h>
#include <stdexcept>

template<typename T>
class CArray {
public:
    CArray(size_t len);
    ~CArray();

    size_t Len() const;

    void Get(size_t start, T *desc, size_t len) const;
    void Put(size_t start, const T *src, size_t len);

    const T & operator[](size_t i) const;
    T & operator[](size_t i);
private:
    CArray(const CArray &);
    void operator=(const CArray &);

    size_t len_;
    T *buffer_;
};

////////////////////////////////////////////////////////////////////////////////
// implement
template<typename T>
CArray<T>::CArray(size_t len) : len_(len) {
    buffer_ = (T *)malloc(sizeof(T) * len);
    if (buffer_ == NULL) {
        throw std::bad_alloc();
    }
}

template<typename T>
CArray<T>::~CArray() {
    free(buffer_);
}

template<typename T>
size_t CArray<T>::Len() const {
    return len_;
}

template<typename T>
void CArray<T>::Get(size_t start, T *desc, size_t len) const {
    if (start < 0 || start + len > len_) {
        throw std::logic_error("CArray::Get() range error");
    }

    memcpy(desc, buffer_ + start, len * sizeof(T));
}

template<typename T>
void CArray<T>::Put(size_t start, const T *src, size_t len) {
    if (start < 0 || start + len > len_) {
        throw std::logic_error("CArray::Put() range error");
    }

    memcpy(buffer_ + start, src, len * sizeof(T));
}

template<typename T>
const T & CArray<T>::operator[](size_t i) const {
    if (i < 0 || i >= len_) {
        throw std::logic_error("CArray::operator[] const range error");
    }

    return *(buffer_ + i);
}

template<typename T>
T & CArray<T>::operator[](size_t i) {
    if (i < 0 || i >= len_) {
        throw std::logic_error("CArray::operator[] range error");
    }

    return *(buffer_ + i);
}

#endif // _CARRAY_H_
