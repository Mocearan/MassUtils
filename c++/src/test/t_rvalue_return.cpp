

/* =============================================================================
* -> FilePath     : /Mass/MassUtils/c++/src/test/t_rvalue_return.cpp
* -> Author       : Mass
* -> Date         : 2020-06-17 16:41:24
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-06-17 16:41:25
* -> Description  : 
* =============================================================================*/

#include <iostream>  
using namespace std;  
  
class A {  
public:  
    int m_k;  
    int m_t;  
    A(int k, int t) :m_k(k), m_t(t) {  
        cout << "construct...." << endl;  
    }  
    ~A() {  
        cout << "destruct...." << endl;  
    }  
    A(const A &a) {  
        cout << "copy construct..." << endl;  
    }  
};
  
//  如果函数返回值是一个对象，要考虑return语句的效率  
A getObj() {  
    /***********  直接返回临时对象  *************/  
    // 编译器直接把临时对象创建并初始化在外部  
    // 存储单元(主调函数的栈帧上)中，省去了拷  
    // 贝和析构的花费，提高了效率  
    /*****************************************/  
    return A(3, 4);               
}  

//  如果函数返回值是一个对象，要考虑return语句的效率  
A getObjc() {  
        /************** 返回本地对象 ****************/      
              /* 以下这种写法实际上执行了三步: 
                  1. 构造本地对象a 
                  2. 调用拷贝构造，将本地对象a拷贝到外部存储器 
                  3. 调用析构函数析构本地对象a 
              */  
         /******************************************/  
    A a(3, 4);  
    cout << "xxx" << endl;
    return a;  
}

int main() {  
    //A a = getObj();   //  外部存储单元  
    A b = getObjc();
    cout << "aaa" << endl;
    return 0;  
}  