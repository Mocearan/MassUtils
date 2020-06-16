

/* =============================================================================
* -> FilePath     : /Mass/MassUtils/c++/src/test/t_openssl.cpp
* -> Author       : Mass
* -> Date         : 2020-06-15 14:44:43
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-06-15 14:44:44
* -> Description  : 
* =============================================================================*/
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>

#include <iostream>
using namespace std;

#include <cstring>
#include <string>

void test_hmac(void)
{
#if 0
	unsigned char md[EVP_MAX_MD_SIZE];
	unsigned int md_len = 0;
	HMAC(EVP_md5(), (const unsigned char*)"jinks", 5, (const unsigned char*)"world", 5, md, &md_len);
	cout << md << endl;
#endif

#if 0
	cout << OPENSSL_API_LEVEL << endl;

	unsigned char a[100] = {'\0'};
	a[0] = 'a';
	a[1] = 'b';
	a[2] = 'c';
	cout << sizeof(a) << endl;
	cout << strlen((char*)a) << endl;
#endif

#if 0
    char key[] = "012345678";
    string data = "hello world";

    const static EVP_MD *(*evp_method_array[])(void) = { 
        EVP_md5, EVP_sha1, EVP_sha256, EVP_sha512
    };  
    const EVP_MD* md = evp_method_array[1]();
    
    unsigned char* mac = new unsigned char(EVP_MAX_MD_SIZE);
    unsigned int mac_length = 0;

    const EVP_MD* engine = evp_method_array[0]();

    HMAC_CTX* ctx_ = HMAC_CTX_new(); //1.1.1
	//HMAC_CTX* ctx_ = new HMAC_CTX; 1.0.2k
    //HMAC_CTX_init(ctx_); 1.0.2k
    HMAC_Init_ex(ctx_, key, strlen(key), engine, NULL);
    HMAC_Update(ctx_, (unsigned char*)data.data(), strlen(data.data()));
    HMAC_Final(ctx_, mac, &mac_length);
    //HMAC_CTX_free(ctx_); //1.1.1
    //HMAC_CTX_cleanup(ctx_); 1.0.2k
	//delete ctx_; 1.0.2k
	ctx_ = NULL;

    for(int i = 0; i < mac_length; i++)
        printf("%-03x", (unsigned int)mac[i]);
    cout << endl;

    delete mac; mac = NULL;
#endif

#if 0
        char key[] = "012345678";
    string data = "hello world";

    const static EVP_MD *(*evp_method_array[])(void) = { 
        EVP_md5, EVP_sha1, EVP_sha256, EVP_sha512
    };  
    
    unsigned char* mac = new unsigned char(EVP_MAX_MD_SIZE);
    size_t mac_length = 0;

    const EVP_MD* engine = evp_method_array[0]();

    EVP_MAC* mac_ = EVP_MAC_fetch(NULL, getenv("MY_MAC"), NULL);
    EVP_MAC_CTX* ctx_ = EVP_MAC_new_ctx(mac_);
    EVP_MAC_init(ctx_);
    EVP_MAC_update(ctx_, (unsigned char*)data.data(), strlen(data.data()));
    EVP_MAC_final(ctx_, mac, &mac_length, sizeof(mac));
    EVP_MAC_free_ctx(ctx_);
    EVP_MAC_free(mac_);

    for(int i = 0; i < mac_length; i++)
        printf("%-03x", (unsigned int)mac[i]);
    cout << endl;

    delete mac; mac = NULL;
#endif
}

void test_1()
{
    string key = "012345678";
    string data = "hello world";

    const static EVP_MD *(*evp_method_array[])(void) = { 
        EVP_md5, EVP_sha1, EVP_sha256, EVP_sha512
    };  
    //const EVP_MD* md = evp_method_array[1]();
    
    unsigned char* mac = new unsigned char(EVP_MAX_MD_SIZE);
    unsigned int mac_length = 0;

    const EVP_MD* engine = evp_method_array[0]();

    HMAC_CTX* ctx_ = new HMAC_CTX;
    HMAC_CTX_init(ctx_);
    HMAC_Init_ex(ctx_, key.data(), data.length(), engine, NULL);
    HMAC_Update(ctx_, (unsigned char*)data.data(), data.length());
    HMAC_Final(ctx_, mac, &mac_length);
    HMAC_CTX_cleanup(ctx_);
    delete ctx_;
     ctx_ = NULL;

    for(int i = 0; i < mac_length; i++)
        printf("%-03x", (unsigned int)mac[i]);
    cout << endl;

    delete mac; mac = NULL;
}


#include "crypto.h"
void test_fact()
{
    unsigned char* mac = new unsigned char(EVP_MAX_MD_SIZE);
    size_t mac_length = 0;

    CCryptoUtil cryp(hmac_utils_type::hmac_ssl, "012345678", hmac_method_flag::h_md5);
    //CCryptoUtil cryp(hmac_utils_type::hmac_ssl, "012345678", hmac_method_flag::h_md5);
    cryp.Update("hello world", 11);
    cryp.Final(mac, mac_length);

    for(int i = 0; i < mac_length; i++)
        printf("%-03x", (unsigned int)mac[i]);
    cout << endl;
}

#include "stringutils.h"
void test_2()
{
    auto mac = CStringUtils::hex(hmac("012345678", "hello world", hmac_method_flag::h_md5));

    for(int i = 0; i < mac.length(); i++)
        i%2 == 0?( i > 0?cout << ' ' << mac[i]:cout << mac[i] ): cout << mac[i];
    cout << endl;
}

auto main(void) -> int
{
    //test_hmac();
    test_1();
    test_fact();

    test_2();
}
