/* =============================================================================
* -> FilePath     : /Mass/MassUtils/c++/src/common/singleton.h
* -> Author       : Mass
* -> Date         : 2020.5.20 14:49:52
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-06-23 13:57:33
* -> Description  : 
*   a no memory leakage and thread-safe singleton template
*   add initor, use by a public int init(void).
*
*@Usage:
*       class A : public Singleton<A> {
            friend class Singleton<A>;
            A() : Singleton(std::bind(&A::init, this)) {...}
        public:
            int init(void);
            virtual ~A(){...}

        ...// class A's own stuff
                ...// class A's own stuff
          // note : if stuff not change after init, safe.
          //      else depend on lock(mutex) keep safe in multi-thread context.
        }
        }
*@Note:
    -lpthread [call_once]
*===================================================================================*/

#ifndef __MASSUTILS_COMMON_SINGLETON_H__
#define __MASSUTILS_COMMON_SINGLETON_H__

#include "noncopyable.h"

#include <atomic>
#include <memory>
#include <mutex>
#include <functional>

BEGIN_NAMESPACE_MASS

template <typename T>
class Singleton : public noncopyable
{
public:
    template <typename... Args>
    static std::shared_ptr<T> GetInstance(Args &&... args)
    {
        std::call_once(
            s_bCalled, [=](Args &&... args) {
                Singleton::s_instance.reset(new T(std::forward<Args>(args)...));
                if (Singleton::bInit and Singleton::s_instance->ini())
                    Singleton::DelInstance();
            },
            std::forward<Args>(args)...);
        return Singleton::s_instance;
    }

    static void DelInstance()
    {
        if (Singleton::s_instance)
        {
            Singleton::s_instance.reset();
        }
    }

protected:
    Singleton()
    {
    }

    template <class StaticInitor>
    Singleton(StaticInitor i) 
    {
        bInit = true;
        //ini = std::bind(i, s_instance);
        ini = i;
    }

    virtual ~Singleton() {}
    
private:
    static std::shared_ptr<T> s_instance;
    static std::once_flag s_bCalled;
    std::function<int(void)> ini;
    //static bool bInit;
    static std::atomic<bool> bInit;
    
};  

template <typename T>
std::shared_ptr<T> Singleton<T>::s_instance = nullptr;

template <typename T>
std::once_flag Singleton<T>::s_bCalled;

// template <typename T>
// std::function<int(void)> Singleton<T>::ini;

template <typename T>
std::atomic<bool> Singleton<T>::bInit = false;

END_NAMESPACE_MASS

#endif // __MASSUTILS_COMMON_SINGLETON_H__
