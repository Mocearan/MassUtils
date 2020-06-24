

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

//#define CRYPTO_REQUIRED

#ifdef CRYPTO_REQUIRED
#include "crypto.h"
#endif

#include "noninstantiable.h"

#include <vector>
#include <iomanip>

BEGIN_NAMESPACE_MASS

class CStringUtils final
    : public noninstantiable
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

    static std::string &replace(std::string &src, char old, char new_);

    static void replace_mark(std::string &str, std::string &new_value, uint32_t &begin_pos);

    static void replace_mark(std::string &str, uint32_t new_value, uint32_t &begin_pos);

    static std::string lower(const std::string &s);

    static std::string upper(const std::string& s);

    static int hexchar2int(char c);

    
    static std::string hex(const char* data, int size);
    static std::string hex(const std::string& data);

    static std::string int2str(uint32_t i);

    static uint32_t str2int(const std::string &value);
    static bool str2int(const std::string &n, int &result, int base = 10);
    static bool str2uint64(const std::string& n, uint64_t& result, int base = 10);
    
    template<typename Inttype>
    static std::string intype2hexstr(const Inttype a)
    {
        //1.
        //char addrAsHex[5] = {(a >> 24) & 0xFF, (a >> 16) & 0xFF, (a >> 8) & 0xFF, a & 0xFF, 0};

        //2. 
        // char buf[20];
        //uint32_t val;
        //sprintf(buf, "\\x%02x\\x%02x\\x%02x\\x%02x", 
        //       (val >> 24), (uint8_t)(val >> 16), (uint8_t)(val >> 8), (uint8_t)val);
        
        std::ostringstream vStream;
        for(std::size_t i = 0 ; i < sizeof(Inttype) ; ++i)
            vStream << "\\x"
                    << std::right << std::setfill('0') << std::setw(2) << std::hex
                    << ((a >> i*4) & 0xFF);

        return vStream.str();
    }


    static bool byteUnit2double(const std::string& n, double& result);
    static bool byteUnit2Int(const std::string& n, int64_t& result);
public:

    static std::string base64_encode(const std::string& src);
    static std::string base64_decode(const std::string& src);

    static std::string url_encode(const std::string& src, bool encode_slash = true);
    static std::string url_decode(const std::string& src);

#ifdef CRYPTO_REQUIRED
    static std::string hmac_sha256_hex(const std::string &key, const std::string &data)
    {
        return hex(hmac(key, data, hmac_method_flag::h_sha256));
    }
#endif
private:
    static inline unsigned char base64_decode_char(const unsigned char);

    const static char* EMPTY_CHARS;
    const static char* BASE64_CHARS;
};

class CStrExplore
{
public:
    CStrExplore(char *str, char seperator);
    virtual ~CStrExplore();

    uint32_t GetItemCnt() { return m_item_cnt; }
    char *GetItem(uint32_t idx) { return m_item_list[idx]; }

private:
    uint32_t m_item_cnt;
    char **m_item_list;
};



END_NAMESPACE_MASS

#endif