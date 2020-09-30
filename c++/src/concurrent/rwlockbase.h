
/* =============================================================================
* -> FilePath     : /MassUtils/c++/src/concurrent/rwlockbase.h
* -> Author       : Mass
* -> Date         : 2020-06-28 15:44:36
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-06-28 15:44:37
* -> Description  : 
            a implement for rwlock base pthread's thread specified data.(pthread)
            不知道好不好用，用来学习读写锁的原理。
* =============================================================================*/

#ifndef __MASSUTILS_CONCURRENT_RWLOCKBASE_H__
#define __MASSUTILS_CONCURRENT_RWLOCKBASE_H__

#include "noncopyable.h"

#include <vector>

BEGIN_NAMESPACE_MASS

class RWLockBase final
    : public noncopyable 
{
public:
    RWLockBase()
    {
        _rwlocks_lock = PTHREAD_MUTEX_INITIALIZER;
        pthread_key_create(&_rwlock_key, NULL);
    }

    ~RWLockBase()
    {
        pthread_key_delete(_rwlock_key);
        for(auto& lock : _rw_locks){
            pthread_mutex_destroy(lock);
            delete lock;
        }
    }

    pthread_mutex_t* r_lock() 
    {
        pthread_mutex_t *lock = (pthread_mutex_t *)pthread_getspecific(_rwlock_key); // 每个线程在调用这个成员函数时，都会产生一个新的线程相关的读锁
        if(not lock) {
            lock = new pthread_mutex_t;
            pthread_mutex_init(lock, NULL);
            pthread_setspecific(_rwlock_key, lock);

            pthread_mutex_lock(&_rwlocks_lock);
            _rw_locks.push_back(lock);
            pthread_mutex_unlock(&_rwlocks_lock);
        }
        pthread_mutex_lock(lock);
        return lock;
    }

    bool r_trylock()
    {
        pthread_mutex_t *lock = (pthread_mutex_t *)pthread_getspecific(_rwlock_key);
        if (not lock)
        {
            lock = new pthread_mutex_t;
            pthread_mutex_init(lock, NULL);
            pthread_setspecific(_rwlock_key, lock);

            if(pthread_mutex_trylock(&_rwlocks_lock))
                return false;
            _rw_locks.push_back(lock);
            pthread_mutex_unlock(&_rwlocks_lock);
        }
        return pthread_mutex_trylock(lock) == 0;
    }

    void r_unlock()
    {
        pthread_mutex_t *lock = (pthread_mutex_t *)pthread_getspecific(_rwlock_key);
        if(lock)
            pthread_mutex_unlock(lock);
    }


/* =============================================================================
    *    写模式独占，需要确认没有线程在操作读锁队列（同时该锁也意味着没有写锁占用）
    *      也需要确认，没有线程在读。（不允许在写的同时读） 
    * =============================================================================*/
    
    void w_lock()
    {
        pthread_mutex_lock(&_rwlocks_lock);
        for(auto& lock : _rw_locks)
            pthread_mutex_lock(lock);
    }

    bool w_trylock()
    {
        if(pthread_mutex_trylock(&_rwlocks_lock))
            return false;
        for (auto &lock : _rw_locks)
            if(pthread_mutex_lock(lock))
                return false;
        return true;
    }

    void w_unlock()
    {
        for(auto& lock : _rw_locks)
            pthread_mutex_unlock(lock);
        pthread_mutex_unlock(&_rwlocks_lock);
    }

private:
    pthread_mutex_t _rwlocks_lock;
    std::vector<pthread_mutex_t *> _rw_locks;

    pthread_key_t _rwlock_key;
};


class RWLockGuard final
{
public:
    RWLockGuard(RWLockBase &base, const char mode = 'r')
        :_lockbase{ base }, _mode{ mode }
    {
        if(mode == 'r')
            _lockbase.r_lock();
        else if(mode == 'w')
            _lockbase.w_lock();
    }

    ~RWLockGuard()
    {
        if(_mode == 'r')
            _lockbase.r_unlock();
        else if (_mode == 'w')
            _lockbase.w_unlock();
    }

    void unlock()
    {
        if (_mode == 'r')
            _lockbase.r_unlock();
        else if (_mode == 'w')
            _lockbase.w_unlock();
    }

private:
    RWLockBase &_lockbase;
    const char _mode;
};

END_NAMESPACE_MASS

#endif // __MASSUTILS_CONCURRENT_RWLOCKBASE_H__