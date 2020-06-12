
#ifndef __MASSUTILS_CPP_COMMON_INCLUDE_NONCOPYABLE_H__
#define __MASSUTILS_CPP_COMMON_INCLUDE_NONCOPYABLE_H__

#include "basedef.h"

BEGIN_NAMESPACE_MASS

class nonimplable {
private:
    nonimplable() = delete;
    ~nonimplable() = delete;
};

END_NAMESPACE_MASS

#endif