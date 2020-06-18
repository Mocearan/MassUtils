

/* =============================================================================
* -> FilePath     : /Mass/MassUtils/c++/src/common/noninstantiable.h
* -> Author       : Mass
* -> Date         : 2020-06-18 11:01:54
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-06-18 13:31:53
* -> Description  : 
* =============================================================================*/


/* =============================================================================
* -> FilePath     : /Mass/MassUtils/c++/src/common/nonimplable.h
* -> Author       : Mass
* -> Date         : 2020-06-11 15:54:32
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-06-17 16:11:30
* -> Description  : 
* =============================================================================*/

#ifndef __MASSUTILS_CPP_COMMON_INCLUDE_NONINSTANTIABLE_H__
#define __MASSUTILS_CPP_COMMON_INCLUDE_NONINSTANTIABLE_H__

#include "basedef.h"

BEGIN_NAMESPACE_MASS

class noninstantiable {
private:
    noninstantiable() = delete;
    ~noninstantiable() = delete;
};

END_NAMESPACE_MASS

#endif // __MASSUTILS_CPP_COMMON_INCLUDE_NONINSTANTIABLE_H__