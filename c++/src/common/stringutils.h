

/* =============================================================================
* -> FilePath     : /Mass/MassUtils/c++/src/common/stringutils.h
* -> Author       : Mass
* -> Date         : 2020-06-11 16:08:13
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-06-16 17:13:31
* -> Description  : 
* =============================================================================*/
#ifndef __MASS_COMMON_STRINGUTILS_H__
#define __MASS_COMMON_STRINGUTILS_H__

#include "crypto.h"
#include "noncopyable.h"

#include <iostream>
#include <sstream>
#include <vector>

class CStringUtils final
    : public Mass::noncopyable, 
     public Mass::nonimplable
{
public:
    template<typename T>
    static std::string to_string(const T& n) {return std::to_string(n);}

    static std::string trim(const std::string& s, const char* c = EMPTY_CHARS);

    static std::string ltrim(const std::string& s, const char* c = EMPTY_CHARS);

    static std::string rtrim(const std::string& s, const char* c = EMPTY_CHARS);

    /*
        TODO:
            update to concept.
    */
    template<typename SequenceStringContainer>
    static void split(const std::string& s, SequenceStringContainer& result, const char* delimiter, int max = 0)
    {
        size_t last = 0;
        result.clear();
        std::string manip = trim(s);
        std::string ss;

        for(int i = 0; not max or i < (max - 1); ++i){
            auto pos = manip.find(delimiter, last);
            if(pos == std::string::npos)
                break;
            ss = manip.substr(last, pos - last);
            if(not ss.empty())
                result.push_back(ss);
            last = pos + 1;
        }
        result.push_back(manip.substr(last));
    }

    static std::string join(const std::vector<std::string>& strs, const std::string& s);

    static std::string lower(const std::string& s);

    static std::string upper(const std::string& s);

    static int hexchar2int(char c);

    
    static std::string hex(const char* data, int size);
    static std::string hex(const std::string& data);

public:

    static std::string base64_encode(const std::string& src);
    static std::string base64_decode(const std::string& src);

    static std::string url_encode(const std::string& src, bool encode_slash = true);
    static std::string url_decode(const std::string& src);

    static std::string hmac_sha256_hex(const std::string& key, const std::string& data)
	{
		return hex(hmac(key, data, hmac_method_flag::h_sha256));
	}

private:
    static inline unsigned char base64_decode_char(const unsigned char);

    const static char* EMPTY_CHARS;
    const static char* BASE64_CHARS;
};





#endif