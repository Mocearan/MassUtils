
/* =============================================================================
* -> FilePath     : /MassUtils/c++/src/test/t_simplelog.cpp
* -> Author       : Mass
* -> Date         : 2020-06-24 17:42:28
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-06-24 17:42:28
* -> Description  : 
* =============================================================================*/

#include "simplelogutil.h"

#include <string>
#include <list>

#include <iostream>

using namespace Mass;

int main()
{
    log(loglevel::DEBUG, "hhhhh");
    log("test info");

    std::string m_pool_name{"unread"};
    std::list<int> m_free_list(2,0);
    log("cache pool: %s, list size: %lu", m_pool_name.data(), m_free_list.size());
    return 0;
}   