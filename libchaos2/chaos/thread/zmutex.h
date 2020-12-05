/*****************************************
**               LibChaos               **
**               zmutex.h               **
**       (c) 2013 Zennix Studios        **
*****************************************/
#ifndef ZMUTEX_H
#define ZMUTEX_H

// ZMutex Class
// WARNING: Relatively untested
// Allows storing of an object in a semi-thread-safe manner.
// Thread-unique locking, so only the thread that locked the mutex is allowed to unlock it.
// While the mutex is locked, other threads may get a refrence to the contained object. THREAD RESPONSIBLY. SERIOUSLY.

#include "ztypes.h"

#include <pthread.h>
#include <atomic>
#include <chrono>

namespace LibChaos {

class ZMutex {
public:
    ZMutex() : locker_tid(0){
        pthread_mutex_init(&mutex, NULL);
    }
    ZMutex(const ZMutex &other) : mutex(other.mutex), locker_tid((pthread_t)other.locker_tid){

    }
    ~ZMutex(){
        pthread_mutex_destroy(&mutex);
    }

    // If mutex is unlocked, mutex is locked by calling thread. If mutex is locked by other thread, function blocks until mutex is unlocked by other thread, then mutex is locked by calling thread.
    void lock(){
        if(!iOwn()){
            pthread_mutex_lock(&mutex);
            locker_tid = pthread_self();
        }
        return; // We own the mutex
    }

    // Locks mutex and returns true if unlocked, else returns false.
    bool trylock(){
        if(!iOwn()){
            if(pthread_mutex_trylock(&mutex) == 0){
                locker_tid = pthread_self();
                return true; // We now own the mutex
            }
            return false; // Another thread owns the mutex
        } else {
            return true; // We already own the mutex
        }
    }

    // Tries to lock the mutex for <timeout_microsec> microseconds, then returns false.
    bool timelock(zu64 timeout_microsec){
        auto end = std::chrono::high_resolution_clock::now() + std::chrono::microseconds(timeout_microsec);
        while(!trylock()){
            // Loop until trylock() succeeds OR timeout is exceeded
            if(std::chrono::high_resolution_clock::now() > end)
                return false; // Timeout exceeded, mutex is locked by another thread
        }
        return true; // Locked, and this thread owns it
    }

    // If mutex is unlocked, returns true. If mutex is locked by calling thread, mutex is unlocked. If mutex is locked by other thread, blocks until mutex is unlocked by other thread.
    void unlock(){
        if(locked()){
            lock(); // Make sure we own the mutex first
            locker_tid = 0;
            pthread_mutex_unlock(&mutex);
        }
    }

    // Returns true if mutex is locked, else returns false.
    inline bool locked(){
        return (bool)locker();
    }
    // Returns locking thread's id, or 0 if unlocked.
    inline pthread_t locker(){
        return locker_tid;
    }
    // Return true if this thread owns the mutex, else returns false
    inline bool iOwn(){
        return (locker() == pthread_self());
    }

    inline pthread_mutex_t *handle(){
        return &mutex;
    }

private:
    pthread_mutex_t mutex;
    std::atomic<pthread_t> locker_tid;
};

// //////////////////////////////////////////////////////////////////////////////

template <class T> class ZMutexV : public ZMutex {
public:
    ZMutexV() : ZMutex(), obj(){}
    ZMutexV(const T &o) : ZMutex(), obj(o){}

    // Block until mutex owned, return refrence to obj
    T &lockdata(){
        lock();
        return obj;
    }

    // Return refrence to obj. Thread responsibly.
    inline T &data(){
        return obj;
    }

private:
    T obj;
};

} // namespace LibChaos

#endif // ZMUTEX_H
