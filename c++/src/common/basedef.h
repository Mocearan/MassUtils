

/* =============================================================================
* -> FilePath     : /Mass/MassUtils/c++/src/common/basedef.h
* -> Author       : Mass
* -> Date         : 2020-05-18 14:46:23
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-05-18 17:38:08
* -> Description  : 
* =============================================================================*/
#ifndef __MASSUTILS_CPP_COMMON_INCLUDE_BASEDEF_H__
#define __MASSUTILS_CPP_COMMON_INCLUDE_BASEDEF_H__

#include <sstream>
#include <map>
#include <string>

#define BEGIN_NAMESPACE(x) \
    namespace x            \
    {
#define END_NAMESPACE(x) }

#define BEGIN_NAMESPACE_MASS BEGIN_NAMESPACE(Mass)
#define END_NAMESPACE_MASS END_NAMESPACE(Mass)

BEGIN_NAMESPACE_MASS

using byte = unsigned char;

using StringBuilder = std::ostringstream;
using StringMap = std::map<std::string, std::string>;

END_NAMESPACE_MASS

#endif //__MASSUTILS_CPP_COMMON_INCLUDE_BASEDEF_H__