#ifndef _NULLLOCK_H_
#define _NULLLOCK_H_

class nulllock {
public:
    inline void lock() {}
    bool try_lock() { return true; }
    inline void unlock() {}
};


#ifdef _WIN32
class spinlock {
public:
    inline spinlock() {
        InitializeCriticalSection(&lock_);
    }

    inline ~spinlock() {
        DeleteCriticalSection(&lock_);
    }

    inline void lock() {
        EnterCriticalSection(&lock_);
    }

    inline bool trylock() {
        return TryEnterCriticalSection(&lock_) != 0;
    }

    inline void unlock() {
        LeaveCriticalSection(&lock_);
    }

private:
    CRITICAL_SECTION lock_;
};
#else // _WIN32
class spinlock {
public:
    inline spinlock() {
        pthread_spin_init(&lock_, 0);
    }

    inline ~spinlock() {
        pthread_spin_destroy(&lock_);
    }

    inline void lock() {
        pthread_spin_lock(&lock_);
    }

    inline bool trylock() {
        return pthread_spin_trylock(&lock_) == 0;
    }

    inline void unlock() {
        pthread_spin_unlock(&lock_);
    }

private:
    pthread_spinlock_t lock_;
};
#endif // _WIN32

template<typename Mutex>
class lock_guard
{
public:
    explicit lock_guard(Mutex& m_) : m(m_) {
        m.lock();
    }
    ~lock_guard() {
        m.unlock();
    }
private:
    Mutex& m;

    explicit lock_guard(lock_guard&);
    lock_guard& operator=(lock_guard&);
};
#endif

