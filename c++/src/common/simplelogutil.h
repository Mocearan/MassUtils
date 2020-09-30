
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
#include "singleton.h"

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

class LogUtil final
{
public:
    static const std::string level_to_string(const loglevel level);

    static loglevel string_to_level(const std::string &level);

    static void logging(loglevel level, const std::string &fmt, ...);

    static void check_and_rotate();

    static inline bool should_log(const loglevel level)
    {
        return level >= s_log_level;
    }


    static inline loglevel GetLogLevel()
    {
        return s_log_level;
    }
    static inline void SetLogLevel(loglevel level)
    {
        s_log_level = level;
    }

    static inline FILE* GetLogStream()
    {
        return s_log_stream;
    }
    static inline void SetLogStream(FILE* fp)
    {
        s_log_stream = fp;
#ifndef _WIN32 //windows not support
        int fd = fileno(fp);
        if (fd < 0)
        {
            return;
        }
        StringBuilder out;
        out << "/proc/self/fd/" << fd;
        char filename[1024];
        int n = readlink(out.str().c_str(), filename, sizeof(filename));
        if (n < 0)
        {
            return;
        }
        filename[n] = 0;
        s_log_filename.clear();
        s_log_filename.append(filename);
#endif
    }

    static inline std::string GetLogFilename()
    {
        return s_log_filename;
    }
    static inline void SetLogFilename(const std::string& name)
    {
        FILE *fp = fopen(name.data(), "a");
        if(not fp)
            return;
        SetLogStream(fp);
        s_log_filename.clear();
        s_log_filename.append(name);
    }
    
    static inline int64_t GetLogRotateSize()
    {
        return s_log_rotate_size;
    }
    static inline void SetLogRotateSize(const int64_t& size)
    {
        s_log_rotate_size = size;
    }

private:
    static loglevel s_log_level;
    static FILE *s_log_stream;
    static std::string s_log_filename;
    static int64_t s_log_rotate_size;
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

void log(const std::string &fmt, ...);

void log(loglevel l);

void log(loglevel l, const std::string &fmt, ...);

END_NAMESPACE_MASS

#endif //__MASSUTILS_COMMON_SIMPLELOGUTILS_H__