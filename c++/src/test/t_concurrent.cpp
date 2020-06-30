
/* =============================================================================
* -> FilePath     : /MassUtils/c++/src/test/t_concurrent.cpp
* -> Author       : Mass
* -> Date         : 2020-06-28 17:08:30
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-06-28 17:08:30
* -> Description  : 
            test concurrent
* =============================================================================*/

#include "rwlockbase.h"

using namespace Mass;

int main(void)
{
    int a;
    RWLockBase base;

    RWLockGuard lk(base);
    a++;

    return 0;
}