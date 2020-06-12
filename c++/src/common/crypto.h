#ifndef __MASS_COMMON_CRYPTOUTILS_H__
#define __MASS_COMMON_CRYPTOUTILS_H__

#include "nonimplable.h"

#include <memory>
#include <string>

enum hmac_method_flag {
	h_md5 = 01,
    h_sha,
	h_sha1,
	h_sha256,
    h_sha383,
	h_sha512
};

class CCryptoUtilsInterface;
using crypto_ptr = std::shared_ptr<CCryptoUtilsInterface>;

class CCryptoFactory final
    : public Mass::nonimplable
{
    enum class crypto_utils_type :char {
        ssl_md5 = 01,
        ssl_sha256,
        ssl_hmac,

        win_md5 = 11,
        win_sha256,
        win_hmac,
    };
public:
    static crypto_ptr GetUtils(crypto_utils_type type);
};


/*
     The following three interfaces are for scenarios with 
        small amounts of data
*/

std::string md5(const std::string& data);
std::string sha256(const std::string& data);
std::string hmac(const std::string& key, const std::string& data, hmac_method_flag flag);



#endif // __MASS_COMMON_CRYPTOUTILS_H__