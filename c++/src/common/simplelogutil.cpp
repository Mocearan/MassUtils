
/* =============================================================================
* -> FilePath     : /MassUtils/c++/src/common/simplelogutil.cpp
* -> Author       : Mass
* -> Date         : 2020-06-28 12:01:12
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-06-28 12:01:12
* -> Description  : 
* =============================================================================*/
#include "simplelogutil.h"

BEGIN_NAMESPACE_MASS

loglevel LogUtil::s_log_level = loglevel::DEBUG;
FILE *LogUtil::s_log_stream = stderr;
std::string LogUtil::s_log_filename;
int64_t LogUtil::s_log_rotate_size = 1024 * 1024 * 1024;

const std::string LogUtil::level_to_string(const loglevel level)
{
    if (level < 0 or level > 4)
        return "DEBUG";

    static std::string lstr[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
    return lstr[level];
}

loglevel LogUtil::string_to_level(const std::string &level)
{
    auto s = CStringUtils::lower(level);
    if ("fatal" == s)
        return loglevel::FATAL;
    else if ("error" == s)
        return loglevel::ERROR;
    else if ("warn" == s)
        return loglevel::WARN;
    else if ("info" == s)
        return loglevel::INFO;
    else if ("debug" == s)
        return loglevel::DEBUG;
    else if ("trace" == s)
        return loglevel::TRACE;

    return loglevel::DEBUG;
}

void LogUtil::logging(loglevel level, const std::string &fmt, ...)
{
    StringBuilder final_fmt;
    int64_t ts = CTimeUtil::now_ms();
    final_fmt << '[' << level_to_string(level) << ']' << ' '
              << CTimeUtil::format("%Y%m%d %H:%M:%S.", ts / 1000)
              << (ts % 1000) << ' '
              << fmt.data()
              << std::endl;
    
    va_list ap;
    va_start(ap, fmt.data());
    vfprintf(s_log_stream, final_fmt.str().data(), ap);
    va_end(ap);

    fflush(s_log_stream);
    check_and_rotate();
}

void LogUtil::check_and_rotate()
{
    if (not s_log_filename.empty())
    {
        if (ftell(s_log_stream) >= s_log_rotate_size)
        {
            StringBuilder out;
            out << s_log_filename
                << '.'
                << CTimeUtil::format("%Y%m%d%H%M%S", time(nullptr));
            if (rename(s_log_filename.data(), out.str().data()) not_eq 0)
                freopen(s_log_filename.data(), "a", s_log_stream);
        }
    }
}

void log(const std::string &fmt, ...)
{
    char temp[64]{'\0'};
    va_list ap;
    va_start(ap, fmt.data());
    vsnprintf(temp, 64, fmt.data(), ap);
    va_end(ap);

    if (LogUtil::should_log(loglevel::INFO))
        LogUtil::logging(loglevel::INFO, std::string("%s:%d ") + temp, __FILE__, __LINE__);
}

void log(loglevel l)
{
    if (LogUtil::should_log(l))
        LogStream(l) << __FILE__ << ':' << __LINE__ << ' ';
}

void log(loglevel l, const std::string &fmt, ...)
{
    va_list ap;
    va_start(ap, fmt.data());

    if (LogUtil::should_log(l))
        LogUtil::logging(l, "%s:%d " + fmt, __FILE__, __LINE__, ap);
    va_end(ap);
}

END_NAMESPACE_MASS