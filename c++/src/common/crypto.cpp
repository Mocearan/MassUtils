

/* =============================================================================
* -> FilePath     : /Mass/MassUtils/c++/src/common/crypto.cpp
* -> Author       : Mass
* -> Date         : 2020-06-15 15:16:17
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-06-15 15:19:18
* -> Description  : 
		-lssl -lcrypto
* =============================================================================*/

#include "crypto.h"
#include "noninstantiable.h"

#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>

#include <cstring>
#include <cassert>


BEGIN_NAMESPACE_MASS

//interface 
class CCryptoUtilsInterface 
{
public:
    virtual ~CCryptoUtilsInterface(){}

    virtual void Update(const void* data, size_t len) = 0;
    virtual void Final(unsigned char* md, size_t& len) = 0;
    //virtual void Init(const std::string& key, const hmac_method_flag flag = hmac_method_flag::null) = 0;
};


// ssl md5 utils definition 
class SslMD5Utils final
    : public CCryptoUtilsInterface
{
public:
	SslMD5Utils() : ctx_{new MD5_CTX} { MD5_Init(ctx_); }
	~SslMD5Utils(){ delete ctx_; }

	virtual void Update(const void* data, size_t len) override 
	{ 
		MD5_Update(ctx_, data, len); 
	}
	
	virtual void Final(unsigned char* md, size_t& len) override
	{
		MD5_Final(md, ctx_);
		len = strlen((char*)md);
	}
	
	//void Init(const std::string& key, const hmac_method_flag flag = hmac_method_flag::null) override;
private:
	MD5_CTX* ctx_;
};


// ssl sha256 utils definition
class SslSHA256Utils final
	: public CCryptoUtilsInterface 
{
public:
	SslSHA256Utils() : ctx_{new SHA256_CTX} { SHA256_Init(ctx_); }
	~SslSHA256Utils(){ delete ctx_; }

	virtual void Update(const void* data, size_t len) override { SHA256_Update(ctx_, data, len); }
	
	virtual void Final(unsigned char* md, size_t& len) override
	{
		SHA256_Final(md, ctx_);
		len = strlen((char*)md);
	}

	//void Init(const std::string& key, const hmac_method_flag flag = hmac_method_flag::null) override;
private:
	SHA256_CTX* ctx_;
};

// ssl hmac utils definition
class SslHMACUtils final
	: public CCryptoUtilsInterface {
public:
	SslHMACUtils(const std::string& key, hmac_method_flag md)
		: ctx_{ new HMAC_CTX }
	{
		HMAC_CTX_init(ctx_);

		const static EVP_MD *(*evp_method_array[])(void) = {
			EVP_md5, EVP_sha, EVP_sha1, EVP_sha256, EVP_sha384, EVP_sha512
		};
		HMAC_Init_ex(ctx_, key.data(), key.length(), evp_method_array[md](), NULL);
	}
	~SslHMACUtils()
	{ 
		HMAC_CTX_cleanup(ctx_); 
		delete ctx_;ctx_=NULL;
	}

	virtual void Update(const void* data, size_t len) override 
	{ 
		HMAC_Update(ctx_, (const unsigned char*)data, len); 	
	}

	virtual void Final(unsigned char* md, size_t& len) override 
	{ 
		HMAC_Final(ctx_, md, (unsigned int*)&len); 
	}
	//void Init(const std::string& key, const hmac_method_flag flag = hmac_method_flag::null) override;
private:
	HMAC_CTX* ctx_;
};



#ifdef _WIN32
#include <wincrypt.h>

class CryptContext {
public:
	CryptContext(LPCWSTR szContainer, LPCWSTR szProvider, DWORD dwProvType, DWORD dwFlags);
	~CryptContext();

	operator HCRYPTPROV (){return _hProv;}

private:
	bool IsWin70Later()
	{
#pragma warning(push)
#pragma warning(disable : 4996)
		DWORD version = GetVersion();
#pragma warning(pop)
		DWORD major = (DWORD)(LOBYTE(LOWORD(version)));
		DWORD minor = (DWORD)(HIBYTE(LOWORD(version)));
		return (major > 6) || ((major == 6) && (minor >= 1));
	}

	HCRYPTPROV _hProv;
};


// win md5 utils definition
class WinMd5Utils final
	: public CCryptoUtilsInterface
{
public:
	WinMd5Utils()
	{
		static CryptContext hProv(NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);

		//specify the hash algorithm 
		if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &_hHash))
			assert(!"CryptCreateHash failed");
	}

	~WinMD5Util() {
        if (_hHash)
            CryptDestroyHash(_hHash);
    }

	virtual void Update(const void* data, size_t len) override
	{
		if (!CryptHashData(_hHash, (const BYTE*)data, (DWORD)len, 0))
			assert(!"CryptHashData failed");
	}

	virtual void Final(unsigned char* md, size_t& len) override
	{
		DWORD cbHashSize = 0;
		DWORD dwCount = sizeof(DWORD);

		//get the final hash size
		if (!CryptGetHashParam(_hHash, HP_HASHSIZE, (BYTE*)&cbHashSize, &dwCount, 0))
			assert(!"CryptGetHashParam failed");

		DWORD dwFinalHashSize = 32;
		//get the computed hash
		if (!CryptGetHashParam(_hHash, HP_HASHVAL, md, &dwFinalHashSize, 0))
			assert(!"CryptGetHashParam failed");

		len = dwFinalHashSize;
	}
	
private:
	HCRYPTHASH _hHash = NULL;
};

// win sha256 utils definition
class WinSha256Utils final
	: public CCryptoUtilsInterface
{
public:
	WinSha256Utils()
	{
		static CryptContext hProv(NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);

		//Specify the Hash Algorithm here
		if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &_hHash))
			assert(!"CryptCreateHash failed");
	}

	~WinSha256Utils()
	{
		if (_hHash) 
			CryptDestroyHash(_hHash);
	}

	virtual void Update(const void* data, size_t len) override 
	{ 
		if (!CryptHashData(_hHash, (const BYTE*)data, (DWORD)len, 0))
			assert(!"CryptHashData failed");
	}

	virtual void Final(unsigned char* md, size_t& len) override
	{
		DWORD cbHashSize = 0;
		DWORD dwCount = sizeof(DWORD);

		//Get the final hash size 
		if (!CryptGetHashParam(_hHash, HP_HASHSIZE, (BYTE*)&cbHashSize, &dwCount, 0)) {
			assert(!"CryptGetHashParam failed");
			return;
		}

		DWORD dwFinalHashSize = 256;
		//Now get the computed hash 
		if (!CryptGetHashParam(_hHash, HP_HASHVAL, md, &dwFinalHashSize, 0)) {
			assert(!"CryptGetHashParam failed");
			return;
		}

		len = dwFinalHashSize;
	}
private:
	HCRYPTHASH _hHash = NULL;
}

// win hmac utils definition
class WinHmacUtils final
	: public CCryptoUtilsInterface
{
public:
	WinHmacUtils(const std::string& key, hmac_method_flag md)
	{
		std::string output;
		static CryptContext hProv(NULL, MS_ENH_RSA_AES_PROV_W, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);

		do {
			BYTE bKeyBuf[1024] = { 0 };
			keyBlob* pBlob = NULL;
			int iBlobLen = 0;
			pBlob = (keyBlob*)bKeyBuf;


			pBlob->hdr.bType = PLAINTEXTKEYBLOB;
			pBlob->hdr.aiKeyAlg = CALG_RC2;
			pBlob->hdr.bVersion = CUR_BLOB_VERSION;
			pBlob->cbKeySize = (DWORD)key.length();
			int keyDataSize = pBlob->cbKeySize;
			memcpy(pBlob->rgbKeyData, key.data(), pBlob->cbKeySize);
			iBlobLen = sizeof(BLOBHEADER) + sizeof(DWORD) + pBlob->cbKeySize;

			if (!CryptImportKey(
				hProv,   // handle of the CSP
				bKeyBuf, // key container name
				iBlobLen, //sizeof(HmacKeyBlob),
				NULL,
				CRYPT_IPSEC_HMAC_KEY,
				&hKey_)) {
				assert(!"CryptImportKey failed");
				break;
			}

			if (!CryptCreateHash(
				hProv,                    // handle of the CSP.
				CALG_HMAC,                // HMAC hash algorithm ID
				hKey_,                     // key for the hash (see above)
				0,                        // reserved
				&hHash_)) {
				assert(!"CryptCreateHash failed");
				break;
			}

			HMAC_INFO   HmacInfo;
			memset(&HmacInfo, 0, sizeof(HmacInfo));

			switch (md)
			{
			case hmac_method_flag::h_md5:
				HmacInfo.HashAlgid = CALG_MD5;
				break;
			case hmac_method_flag::h_sha1:
				HmacInfo.HashAlgid = CALG_SHA1;
				break;
			case hmac_method_flag::h_sha256:
				HmacInfo.HashAlgid = CALG_SHA_256;
				break;
			case hmac_method_flag::h_sha512:
				HmacInfo.HashAlgid = CALG_SHA_512;
				break;
			default:
				HmacInfo.HashAlgid = CALG_SHA_256;
				break;
			}

			if (!CryptSetHashParam(
				hHash_,                // handle of the HMAC hash object
				HP_HMAC_INFO,             // setting an HMAC_INFO object
				(BYTE*)&HmacInfo,         // the HMAC_INFO object
				0)) {
				assert(!"CryptSetHashParam failed");
				break;
			}
		} while (0);
	}

	~WinHmacUtils()
	{
		if (hHash_) {
			CryptDestroyHash(hHash_);
			hHash_ = NULL;
		}
		if (hKey_) {
			CryptDestroyKey(hKey_);
			hKey_ = NULL;
		}
	}

	virtual void Update(const void* data, size_t len) override 
	{ 
		if (!CryptHashData(
			hHash_,                // handle of the HMAC hash object
			(BYTE*)data,                    // message to hash
			(DWORD)len,            // number of bytes of data to add
			0)) 
			assert(!"CryptHashData failed");	
	}

	virtual void Final(unsigned char* md, size_t& len) override 
	{ 
		DWORD dwDataLen = 0;
		if (!CryptGetHashParam(
			hHash_,                // handle of the HMAC hash object
			HP_HASHVAL,               // query on the hash value
			NULL,                     // filled on second call
			&dwDataLen,               // length, in bytes, of the hash
			0)) {
			return;
		}

		if (!CryptGetHashParam(
			hHash_,                 // handle of the HMAC hash object
			HP_HASHVAL,                // query on the hash value
			(BYTE*)md,                    // pointer to the HMAC hash value
			&dwDataLen,                // length, in bytes, of the hash
			0)) {
			assert(!"CryptGetHashParam failed");
			return;
		}

		len = dwDataLen;
	}

private:
	HCRYPTKEY hKey_ = NULL;
	HCRYPTHASH hHash_ = NULL;

	typedef struct _keyBlob {
		BLOBHEADER hdr;
		DWORD cbKeySize;
		BYTE rgbKeyData[256];
	} keyBlob;
}

#endif 

// utils factory
class CCryptoFactory final
    : public Mass::noninstantiable
{
public:
    static crypto_ptr GetUtils(crypto_utils_type type)
	{
		switch (type)
		{
		case crypto_utils_type::ssl_md5:
			return std::make_shared<SslMD5Utils>();
		case crypto_utils_type::ssl_sha256:
			return std::make_shared<SslSHA256Utils>();
#ifdef _WIN32
		case crypto_utils_type::win_md5:
			return std::make_shared<WinMd5Utils>();
		case crypto_utils_type::win_sha256:
			return std::make_shared<WinSha256Utils>();
#endif
		default:
			return nullptr;
		}
	}


    static crypto_ptr GetUtils(hmac_utils_type type
			, const std::string& hmac_key , const hmac_method_flag& md)
	{
		switch (type)
		{
		case hmac_utils_type::hmac_ssl:
			return std::make_shared<SslHMACUtils>(hmac_key, md);
#if _WIN32
		case hmac_utils_type::hmac_win:
			return std::make_shared<WinHmacUtils>(hmac_key, md);
#endif
		default:
			return nullptr;
		}
	}
};

CCryptoUtil::CCryptoUtil(const crypto_utils_type& type)
{
	p_cry = CCryptoFactory::GetUtils(type);
}

CCryptoUtil::CCryptoUtil(const hmac_utils_type& type, const std::string& key, hmac_method_flag md) 
{
	assert(not key.empty());
	p_cry = CCryptoFactory::GetUtils(type, key, md);
}

CCryptoUtil::~CCryptoUtil(){}


void CCryptoUtil::Update(const void* data, size_t len) 
{
	p_cry->Update(data, len);
}

void CCryptoUtil::Final(unsigned char* md, size_t& len)
{
	p_cry->Final(md, len);
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
		HMAC(EVP_md5(), (const unsigned char*)key.data(), key.length(), (const unsigned char*)data.data(), data.size(), md, &md_len);
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


END_NAMESPACE_MASS