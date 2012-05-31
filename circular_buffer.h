#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

#include "array.h"
#include "lock.h"

template<typename T, typename RLOCK, typename WLOCK>
class CircularBuffer {
public:
    CircularBuffer(size_t len);

    int Put(const T *items, size_t count);
    int Get(T *items, size_t count);
    int Peek(T *items, size_t count);

private:
    int UnlockPeek(T *items, size_t count) const;

    CArray<T> buffer_;

    size_t readpos_;
    size_t writepos_;

    RLOCK rlock_;
    WLOCK wlock_;
};

////////////////////////////////////////////////////////////////////////////////
// implement
template<typename T, typename RLOCK, typename WLOCK>
CircularBuffer<T, RLOCK, WLOCK>::CircularBuffer(size_t len) 
        : buffer_(len),
          readpos_(0),
          writepos_(0) {
}

template<typename T, typename RLOCK, typename WLOCK>
int CircularBuffer<T, RLOCK, WLOCK>::Put(const T *items, size_t count) {
    lock_guard<WLOCK> guard(wlock_);
    const size_t remain = (readpos_ + buffer_.Len() - writepos_ - 1) % buffer_.Len();
    if (count > remain) {
        return -1;
    }
    const size_t rewind = buffer_.Len() - writepos_;
    if (rewind >= count) {
        buffer_.Put(writepos_, items, count);
    } else {
        buffer_.Put(writepos_, items, rewind);
        buffer_.Put(0, items + rewind, count - rewind);
    }
    writepos_ = (writepos_ + count) % buffer_.Len();
    return 0;
}

template<typename T, typename RLOCK, typename WLOCK>
int CircularBuffer<T, RLOCK, WLOCK>::Get(T *items, size_t count) {
    lock_guard<RLOCK> guard(rlock_);
    const int rv = UnlockPeek(items, count);
    if (rv == 0) {
        readpos_ = (readpos_ + count) % buffer_.Len();
    }
    return rv;
}

template<typename T, typename RLOCK, typename WLOCK>
int CircularBuffer<T, RLOCK, WLOCK>::Peek(T *items, size_t count) {
    lock_guard<RLOCK> guard(rlock_);
    return UnlockPeek(items, count);
}

template<typename T, typename RLOCK, typename WLOCK>
inline int CircularBuffer<T, RLOCK, WLOCK>::UnlockPeek(T *items, size_t count) const {
    const size_t remain = (writepos_ + buffer_.Len() - readpos_) % buffer_.Len();
    if (remain < count) {
        return -1;
    }
    const size_t rewind = buffer_.Len() - readpos_;
    if (rewind >= count) {
        buffer_.Get(readpos_, items, count);
    } else {
        buffer_.Get(readpos_, items, rewind);
        buffer_.Get(0, items + rewind, count - rewind);
    }
    return 0;
}

#endif //_CIRCULAR_BUFFER_H_
