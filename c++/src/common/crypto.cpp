
#include "crypto.h"


#if OPENSSL_API_LEVEL >= 30000
	#include <openssl/mac.h>
#else
	#include <openssl/hmac.h>
	#include <openssl/evp.h>
#endif

#include <openssl/md5.h>
#include <openssl/sha.h>

#include <cstring>

//interface 
class CCryptoUtilsInterface {
public:
    virtual ~CCryptoUtilsInterface(){}

    virtual void Update(void* data, size_t len) = 0;
    virtual void Final(unsigned char* md, unsigned int& len) = 0;
    //virtual void Init(const std::string& key, const hmac_method_flag flag = hmac_method_flag::null) = 0;
    virtual void Clear() = 0;
};


// md5 utils definition 
class SslMD5Utils final
    : public CCryptoUtilsInterface
{
public:
	SslMD5Utils();
	~SslMD5Utils();

	virtual void Update(void* data, size_t len) override;
	virtual void Final(unsigned char* md, unsigned int& len) override;
	void Clear() override;
	//void Init(const std::string& key, const hmac_method_flag flag = hmac_method_flag::null) override;
private:
	MD5_CTX* ctx_ = new MD5_CTX;
};


// sha256 utils definition
class SslSHA256Utils final
	: public CCryptoUtilsInterface 
{
public:
	SslSHA256Utils();
	~SslSHA256Utils();

	virtual void Update(void* data, size_t len) override;
	virtual void Final(unsigned char* md, unsigned int& len) override;

	void Clear() override;
	//void Init(const std::string& key, const hmac_method_flag flag = hmac_method_flag::null) override;
private:
	SHA256_CTX* ctx_ = new SHA256_CTX;
};


//hmac utils definition
class SslHMACUtils : public CCryptoUtilsInterface {
public:
	SslHMACUtils(const std::string& key, hmac_method_flag md);
	~SslHMACUtils();

	virtual void Update(void* data, size_t len) override;
	virtual void Final(unsigned char* md, unsigned int& len) override;

	void Clear() override;
	//void Init(const std::string& key, const hmac_method_flag flag = hmac_method_flag::null) override;
private:
	HMAC_CTX* ctx_ = HMAC_CTX_new();
};



SslMD5Utils::SslMD5Utils()
{
	MD5_Init(ctx_);
}

SslMD5Utils::~SslMD5Utils() 
{
	delete ctx_;
}


void SslMD5Utils::Update(void* data, size_t len)
{
	MD5_Update(ctx_, data, len);
}

void SslMD5Utils::Final(unsigned char* md, unsigned int& len)
{
	MD5_Final(md, ctx_);
	len = strlen((char*)md);
}

void SslMD5Utils::Clear()
{
	delete ctx_; // delete inner if(null)
	ctx_ = new MD5_CTX;
}



SslSHA256Utils::SslSHA256Utils()
{
	SHA256_Init(ctx_);
}

SslSHA256Utils::~SslSHA256Utils() 
{
	delete ctx_;
}


void SslSHA256Utils::Update(void* data, size_t len)
{
	SHA256_Update(ctx_, data, len);
}

void SslSHA256Utils::Final(unsigned char* md, unsigned int& len)
{
	SHA256_Final(md, ctx_);
	len = strlen((char*)md);
}

void SslSHA256Utils::Clear()
{
	delete ctx_;
	ctx_ = new SHA256_CTX;
}

SslHMACUtils::SslHMACUtils(const std::string& key, hmac_method_flag md)
{
	const EVP_MD *(*evp_method_array[])(void) = {
		NULL, EVP_md5, EVP_sha1, EVP_sha256, EVP_sha512
	};
	HMAC_Init_ex(ctx_, key.data(), key.length(), evp_method_array[md](), NULL);
}

SslHMACUtils::~SslHMACUtils()
{
	HMAC_CTX_free(ctx_);
}

void SslHMACUtils::Update(void* data, size_t len)
{
	HMAC_Update(ctx_, (const unsigned char*)data, len);
}

void SslHMACUtils::Final(unsigned char* md, unsigned int& len)
{
	HMAC_Final(ctx_, md, &len);
}

void SslHMACUtils::Clear()
{
	HMAC_CTX_reset(ctx_);
}



crypto_ptr CCryptoFactory::GetUtils(crypto_utils_type type) 
{
    switch (type)
	{
	case crypto_utils_type::ssl_md5:
		return std::make_shared<SslMD5Utils>();
	case crypto_utils_type::ssl_sha256:
		return std::make_shared<SslSHA256Utils>();
	case crypto_utils_type::ssl_hmac:
		//return new SslHMACUtils();
	case crypto_utils_type::win_md5:
		//return new WinMD5Utils();
	case crypto_utils_type::win_sha256:
		//return new WinSHA256Utils();
	case crypto_utils_type::win_hmac:
		//return new WinHMACUtils();
	default:
		return NULL;
	}
}





std::string md5(const std::string& data)
{
	unsigned char md[MD5_DIGEST_LENGTH];
	MD5((const unsigned char*)data.data(), data.size(), md);
	return std::string((char*)md);
}


std::string sha256(const std::string& data)
{
	unsigned char md[SHA256_DIGEST_LENGTH];
	SHA256((const unsigned char*)data.data(), data.size(), md);
	return std::string((char*)md);
}

std::string hmac(const std::string& key, const std::string& data, hmac_method_flag flag)
{
	unsigned char md[EVP_MAX_MD_SIZE];
	unsigned int md_len = 0;
	switch (flag)
	{
	case hmac_method_flag::h_md5:
		HMAC(EVP_md5(), (const unsigned char*)key.data(), key.size(), (const unsigned char*)data.data(), data.size(), md, &md_len);
		break;
	case hmac_method_flag::h_sha1:
		HMAC(EVP_sha1(), (const unsigned char*)key.data(), key.size(), (const unsigned char*)data.data(), data.size(), md, &md_len);
		break;
	case hmac_method_flag::h_sha256:
		HMAC(EVP_sha256(), (const unsigned char*)key.data(), key.size(), (const unsigned char*)data.data(), data.size(), md, &md_len);
		break;
	case hmac_method_flag::h_sha512:
		HMAC(EVP_sha512(), (const unsigned char*)key.data(), key.size(), (const unsigned char*)data.data(), data.size(), md, &md_len);
		break;
	default:
		HMAC(EVP_md5(), (const unsigned char*)key.data(), key.size(), (const unsigned char*)data.data(), data.size(), md, &md_len);
		break;
	}
	return std::string((char*)md, md_len);
}


