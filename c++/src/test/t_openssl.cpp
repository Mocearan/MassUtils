#include <openssl/hmac.h>
#include <openssl/evp.h>

#include <iostream>
using namespace std;

#include <cstring>
#include <string>

auto main(void) -> int
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
    HMAC_CTX_free(ctx_); //1.1.1
    //HMAC_CTX_cleanup(ctx_); 1.0.2k
	//delete ctx_; 1.0.2k
	ctx_ = NULL;

    for(int i = 0; i < mac_length; i++)
        printf("%-03x", (unsigned int)mac[i]);
    cout << endl;

    delete mac; mac = NULL;

}
