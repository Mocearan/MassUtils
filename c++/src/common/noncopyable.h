
#ifndef __MASSUTILS_CPP_COMMON_NONCOPYABLE_H__
#define __MASSUTILS_CPP_COMMONE_NONCOPYABLE_H__

#include "basedef.h"

BEGIN_NAMESPACE_MASS

class noncopyable {
protected:
    noncopyable(){}
    ~noncopyable(){}

private:
    explicit noncopyable(const noncopyable&) = delete;
    const noncopyable& operator=(const noncopyable&) = delete;
};

END_NAMESPACE_MASS

#endif