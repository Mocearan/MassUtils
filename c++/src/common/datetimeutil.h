

/* =============================================================================
* -> FilePath     : /Mass/MassUtils/c++/src/common/datetimeutil.h
* -> Author       : Mass
* -> Date         : 2020-06-17 14:04:35
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-06-17 14:04:36
* -> Description  : 
* =============================================================================*/
#ifndef __MASS_COMMON_CRYPTOUTILS_H__
#define __MASS_COMMON_CRYPTOUTILS_H__


#include "noninstantiable.h"

#include <ctime>
#include <string>

BEGIN_NAMESPACE_MASS

class CDateUtil final
    : public noninstantiable
{
public:
    static time_t parse_rfc822_date(const std::string &date);
    static time_t parse_iso8601_date(const std::string& date);
    static std::string format_rfc822_date(time_t timestamp);
    static std::string format_iso8601_date(time_t timestamp);
};

class CTimeUtil final
    : public noninstantiable
{
public:
    static int64_t now_ms();
    static int64_t now_us();
    static time_t now();
    static struct tm now_tm();
    static std::string now_utctime();
    static std::string now_gmttime();
    
    static std::string format(const std::string& fmt, time_t ts);
    static std::string timestamp_to_utctime(const time_t&);
    static std::string timestamp_to_gmttime(const time_t&);
    static int64_t utctime_to_timestamp(const std::string&);
    static int64_t gmttime_to_timestamp(const std::string&);

    static const char* UTC_FORMAT;
    static const char* GMT_FORMAT;
    static const int UTC_FORMAT_LENGTH;
};

#ifdef _WIN32 

int gettimeofday(struct timeval* tp, void* tzp);
void gmtime_r(const time_t* time, struct tm* tm_struct);
void localtime_r(const time_t, struct tm* tm_struct);
char* strptime(const char* buf, const char* fmt, struct tm* tm);

#define timegm _mkgmtime

#endif // _WIN32

#endif // __MASS_COMMON_CRYPTOUTILS_H__

END_NAMESPACE_MASS