/*
*@Author    Mass
*@Date      2020.5.20
*@Desc:
*   a no memory leakage and thread-safe singleton template
*@Usage:
*       class A : public Singleton<A> {
            friend class Singleton<A>;
            A(){...}
        public:
            virtual ~A(){...}
        ...// class A's own stuff
        }
*@Note:
    -lpthread [call_once]
*/

#ifndef __MASSUTILS_COMMON_SINGLETON_H__
#define __MASSUTILS_COMMON_SINGLETON_H__

#include "noncopyable.h"

#include <memory>
#include <mutex>
#include <functional>

#include <iostream>
using namespace std;



BEGIN_NAMESPACE_MASS

template<typename T>
class Singleton  : public noncopyable {
public:

    template<typename ...Args>
    static std::shared_ptr<T> GetInstance(Args&&... args) 
    {
        std::call_once(s_bCalled, [=]( Args&&... args){
            Singleton::s_instance.reset(new T(std::forward<Args>(args)...));
        }, std::forward<Args>(args)...);
        return Singleton::s_instance;
    }

    
    static void DelInstance() {
        if(Singleton::s_instance) {
            Singleton::s_instance.reset(nullptr);
        }
    }
    
protected:

    Singleton()	{}
    
    virtual ~Singleton() {}
    
private:
    static std::shared_ptr<T> s_instance;
    static std::once_flag s_bCalled;
};

template<typename T>
std::shared_ptr<T> Singleton<T>::s_instance = nullptr;

template<typename T>
std::once_flag  Singleton<T>::s_bCalled;

END_NAMESPACE_MASS

#endif // __MASSUTILS_COMMON_SINGLETON_H__
