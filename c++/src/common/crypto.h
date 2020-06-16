

/* =============================================================================
* -> FilePath     : /Mass/MassUtils/c++/src/common/crypto.h
* -> Author       : Mass
* -> Date         : 2020-06-15 14:52:17
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-06-15 14:52:26
* -> Description  : 
* =============================================================================*/
#ifndef __MASS_COMMON_CRYPTOUTILS_H__
#define __MASS_COMMON_CRYPTOUTILS_H__

#include "nonimplable.h"

#include <memory>
#include <string>

enum hmac_method_flag {
	h_md5,
    h_sha,
	h_sha1,
	h_sha256,
    h_sha384,
	h_sha512
};

enum class crypto_utils_type :char {
    ssl_md5 = 01,
    ssl_sha256,
#ifdef _WIN32
    win_md5 = 11,
    win_sha256,
#endif
};

enum class hmac_utils_type :char{
    hmac_ssl,
#ifdef _WIN32
    hmac_win,
#endif
};

class CCryptoUtilsInterface;
using crypto_ptr = std::shared_ptr<CCryptoUtilsInterface>;


class CCryptoUtil final
{
public:
    CCryptoUtil(const crypto_utils_type& type);
    CCryptoUtil(const hmac_utils_type& type, const std::string& key, hmac_method_flag md = hmac_method_flag::h_sha256);
    ~CCryptoUtil();

    void Update(const void* data, size_t len);
    void Final(unsigned char* md, size_t& len);
private:
    crypto_ptr p_cry;
};


/*
     The following three interfaces are for scenarios with 
        small amounts of data
*/

std::string md5(const std::string& data);
std::string sha256(const std::string& data);
std::string hmac(const std::string& key, const std::string& data, hmac_method_flag flag);



#endif // __MASS_COMMON_CRYPTOUTILS_H__