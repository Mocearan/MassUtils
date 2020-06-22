
/* =============================================================================
* -> FilePath     : /MassUtils/c++/src/common/simplelogutils.hpp
* -> Author       : Mass
* -> Date         : 2020-06-22 14:54:05
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-06-22 14:54:06
* -> Description  : 
* =============================================================================*/

/* =============================================================================
* -> FilePath     : /MassUtils/c++/src/common/simplelogutils.hpp
* -> Author       : Mass
* -> Date         : 2020-06-19 22:03:29
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-06-19 22:03:29
* -> Description  : 
* =============================================================================*/

#ifndef __MASSUTILS_COMMON_SIMPLELOGUTILS_H__
#define __MASSUTILS_COMMON_SIMPLELOGUTILS_H__

#include "stringutils.h"
#include "datetimeutil.h"

#include <cstdarg>

#include <unistd.h>

BEGIN_NAMESPACE_MASS

enum loglevel{
    TRACE = 0,
    DEBUG ,
    INFO ,
    WARN ,
    ERROR ,
    FATAL ,
};

void set_log_level(loglevel level);

/* =============================================================================
* -> Description: if using stream, will convert stream to filename by readlink
* -> param {type} 
* -> return: 
* =============================================================================*/
void set_log_stream(FILE *fp);

void set_log_filename(const std::string &filename);

void set_log_rotate_size(int size);

void log(const string &fmt, ...)
{
    va_list ap;
    va_start(ap, fmt.data());

    if (LogUtil::should_log(loglevel::INFO))
        LogUtil::logging(loglevel::INFO, "%s:%d " + fmt, __FILE__, __LINE__, ap);
    va_end(ap);
}

void log(loglevel l)
{
    if (LogUtil::should_log(l))
        LogStream(l) << __FILE__ << ':' << __LINE__ << ' ';
}

void log(loglevel l, const string &fmt, ...)
{
    va_list ap;
    va_start(ap, fmt.data());
    
    if(LogUtil::should_log(l))
        LogUtil::logging(l, "%s:%d " + fmt, __FILE__, __LINE__, ap);
    va_end(ap);
}

class LogUtil {
public:
    static const string level_to_string(const loglevel level);
    static loglevel string_to_level(const std::string &level);

    static void logging(loglevel level, const string &fmt, ...);

    static void check_and_rotate();

    static inline bool should_log(const loglevel level)
    {
        return level <= Provider::GetInstance()->s_log_level();
    }


    static inline loglevel GetLogLevel()
    {
        return Provider::GetInstance()->s_log_level();
    }
    static inline void SetLogLevel(loglevel level)
    {
        Provider::GetInstance()->s_log_level() = level;
    }

    static inline FILE* GetLogStream()
    {
        return Provider::GetInstance()->s_log_stream();
    }
    static inline void SetLogStream(FILE* fp)
    {
        Provider::GetInstance()->set_s_log_stream(fp);
#ifndef _WIN32 //windows not support
        int fd = fileno(fp);
        if (fd < 0)
        {
            return;
        }
        std::ostringstream out;
        out << "/proc/self/fd/" << fd;
        char filename[1024];
        int n = readlink(out.str().c_str(), filename, sizeof(filename));
        if (n < 0)
        {
            return;
        }
        filename[n] = 0;
        Provider::GetInstance()->s_log_filename().clear();
        Provider::GetInstance()->s_log_filename().append(filename);
#endif
    }

    static inline std::string GetLogFilename()
    {
        return Provider::GetInstance()->s_log_filename();
    }
    static inline void SetLogFilename(const string& name)
    {
        FILE *fp = fopen(name.data(), "a");
        if(not fp)
            return;
        SetLogStream(fp);
        Provider::GetInstance()->s_log_filename().clear();
        Provider::GetInstance()->s_log_filename().append(name);
    }
    
    static inline int64_t GetLogRotateSize()
    {
        return Provider::GetInstance()->s_log_rotate_size();
    }
    static inline void SetLogRotateSize(const int64_t& size)
    {
        Provider::GetInstance()->s_log_rotate_size() = size;
    }

private:

    // static LogLevel s_log_level;
    // static FILE *s_log_stream;
    // static std::string s_log_filename;
    // static int64_t s_log_rotate_size;

    // chang static for hpp.
    class Provider : public Singleton<Provider>{
    public:
        loglevel& s_log_level()
        {
            return log_level_;
        }

        FILE* s_log_stream()
        {
            return log_stream_;
        }

        void set_s_log_stream(FILE* fp)
        {
            log_stream_ = fp;
        }

        std::string& s_log_filename()
        {
            return log_filename_;
        }

        int64_t& s_log_rotate_size()
        {
            return log_rotate_size_;
        }

    private:
        loglevel log_level_ = loglevel::DEBUG;
        FILE *log_stream_ = stderr;
        std::string log_filename_;
        int64_t log_rotate_size_ = 1024*1024*1024;
    };
};

class LogStream{
public:
    LogStream(const loglevel level) : m_level(level){};
    ~LogStream()
    {
        LogUtil::logging(m_level, "%s", m_stream.str().data());
    }

    template<typename T>
    inline LogStream& operator <<(const T& t)
    {
        m_stream << t;
        return *this;
    }

private:
    loglevel m_level;
    StringBuilder m_stream;
};

const string LogUtil::level_to_string(const loglevel level)
{
    if(level < 0 or level > 4)
        return "DEBUG";

    static string lstr[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
    return lstr[level];
}

loglevel LogUtil::string_to_level(const std::string &level)
{
    auto s = CStringUtils::lower(level);
    if("fatal" == s)
        return loglevel::FATAL;
    else if("error" == s)
        return loglevel::ERROR;
    else if("warn" == s)
        return loglevel::WARN;
    else if("info" == s)
        return loglevel::INFO;
    else if("debug" == s)
        return loglevel::DEBUG;
    else if("trace" == s)
        return loglevel::TRACE;

    return loglevel::DEBUG;
}

void LogUtil::logging(loglevel level, const string& fmt, ...)
{
    StringBuilder final_fmt;
    int64_t ts = CTimeUtil::now_ms();
    final_fmt << '[' << level_to_string(level) << '] '
              << CTimeUtil::format("%Y%m%d %H:%M:%S.", ts / 1000)
              << (ts % 1000) << ' '
              << fmt.data()
              << std::endl;

    va_list ap;
    va_start(ap, fmt.data());
    vfprintf(Provider::GetInstance()->s_log_stream(), final_fmt.str().data(), ap);
    va_end(ap);

    fflush(Provider::GetInstance()->s_log_stream());
    check_and_rotate();
}

void LogUtil::check_and_rotate()
{
    if (not Provider::GetInstance()->s_log_filename().empty())
    {
        if (ftell(Provider::GetInstance()->s_log_stream()) >= Provider::GetInstance()->s_log_rotate_size())
        {
            StringBuilder out;
            out << Provider::GetInstance()->s_log_filename()
                << '.'
                << CTimeUtil::format("%Y%m%d%H%M%S", time(nullptr));
            if(rename(Provider::GetInstance()->s_log_filename().data(), out.str().data())
                not_eq 0 )
                freopen(Provider::GetInstance()->s_log_filename().data(), "a", Provider::GetInstance()->s_log_stream());
        }
    }
}

END_NAMESPACE_MASS

#endif //__MASSUTILS_COMMON_SIMPLELOGUTILS_H__