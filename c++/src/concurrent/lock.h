
/* =============================================================================
* -> FilePath     : /MassUtils/c++/src/concurrent/lock.h
* -> Author       : Mass
* -> Date         : 2020-06-28 15:24:24
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-06-28 15:24:25
* -> Description  : 
                Wrapper for pthread
* =============================================================================*/

#ifndef __MASSUTILS_CONCURRENT_LOCK_H__
#define __MASSUTILS_CONCURRENT_LOCK_H__

#include "noncopyable.h"

#include <pthread.h>

BEGIN_NAMESPACE_MASS

class CMLock final 
    : public noncopyable                      
{
public:
    CMLock(){}
    ~CMLock(){}

    void lock() { pthread_mutex_lock(&_mutex); }

    void unlock() { pthread_mutex_unlock(&_mutex); }

    bool trylock() { return (0 ==  pthread_mutex_trylock(&_mutex)); }

    const pthread_mutex_t &GetMutex() const { return _mutex; }

private:
    pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER;
};

class CMRWLock final
    : public noncopyable
{
public:
    CMRWLock(){}
    ~CMRWLock(){}

    void rlock() { pthread_rwlock_rdlock(&_rwlock); }
    void wlock() { pthread_rwlock_wrlock(&_rwlock); }
    void unlock() { pthread_rwlock_unlock(&_rwlock); }

    bool try_rlock() { return (0 == pthread_rwlock_tryrdlock(&_rwlock)); }
    bool try_wlock() { return (0 == pthread_rwlock_trywrlock(&_rwlock)); }

private:
    pthread_rwlock_t _rwlock = PTHREAD_RWLOCK_INITIALIZER;
};

class CMLockGuard final
    : public noncopyable
{
public:
    CMLockGuard(CMLock &lk) : _lock{lk}
    {
        _lock.lock();
    }

    ~CMLockGuard()
    {
        CMLock.unlock();
    }

private:
    CMLock &_lock;
};

class CMRWLockGuard final
    : public noncopyable
{
public:
    CMRWLockGuard(CMRWLock &rwlk, const char mode = 'r')
        : _rwlock{rwlk}, _mode{mode}
    {
        if (mode == 'r')
            _rwlock.r_lock();
        else if (mode == 'w')
            _rwlock.w_lock();
    }

    ~CMRWLockGuard()
    {
        if (_mode == 'r')
            _rwlock.r_unlock();
        else if (_mode == 'w')
            _rwlock.w_unlock();
    }

private:
    CMRWLock &_rwlock;
    const char _mode;
};

END_NAMESPACE_MASS

#endif // __MASSUTILS_CONCURRENT_LOCK_H__