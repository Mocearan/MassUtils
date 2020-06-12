#include "singleton.h"

#include <iostream>
#include <string>
using namespace std;

using namespace Mass;

class A final
    : public Singleton<A>
{
    friend class Singleton<A>;
    A(){init(); cout << this << endl;}
public:
    virtual ~A(){}

     void init()
     {
         cout << "init" << endl;
         while(a < 100000000)
            a++;
     }


     void show()
     {
         cout <<"show " << this << " " << a << endl;
     }

private:
    int a = 20;
};

class B 
    : public Singleton<B>
{
    friend class Singleton<B>;
    B(const string& str) : b_{str} {  cout << this << " " << b_ << endl; }
public:
    virtual ~B(){}

    void show()
    {
        cout << "show " << this << " " << b_ << endl;
    }

private:
    string b_;
};


int main(void)
{
    A::GetInstance();
    //A::GetInstance(12); // no match constructor
    A::GetInstance()->show();
    A::GetInstance()->show();

    B::GetInstance("this")->show();
    B::GetInstance("this")->show();
    B::GetInstance("this")->show();
    B::GetInstance("this")->show();

    return 0;
}