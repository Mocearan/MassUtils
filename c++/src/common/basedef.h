#ifndef __MASSUTILS_CPP_COMMON_INCLUDE_BASEDEF_H__
#define __MASSUTILS_CPP_COMMON_INCLUDE_BASEDEF_H__

#define BEGIN_NAMESPACE(x) namespace x {
#define END_NAMESPACE(x) }

#define BEGIN_NAMESPACE_MASS BEGIN_NAMESPACE(Mass)
#define END_NAMESPACE_MASS  END_NAMESPACE(Mass)

using byte = unsigned char;

#include <sstream>
using StringBuilder = std::ostringstream;

#endif //__MASSUTILS_CPP_COMMON_INCLUDE_BASEDEF_H__