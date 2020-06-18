#include "stringutils.h"

#include <climits>
#include <iomanip>


BEGIN_NAMESPACE_MASS

// String Utils
const char* CStringUtils::EMPTY_CHARS = " \n\t\r\v\f";
const char* CStringUtils::BASE64_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";



std::string CStringUtils::trim(const std::string& s, const char* c)
{
    size_t b = s.find_first_not_of(c);
    if(b == std::string::npos)
        return "";
    
    size_t e = s.find_last_not_of(c);
    if(e == std::string::npos)
        return s.substr(b);

    return s.substr(b, e - b + 1);
}

std::string CStringUtils::ltrim(const std::string& s, const char* c)
{
    size_t b = s.find_first_not_of(c);
    if(b == std::string::npos)
        return s;
    return s.substr(b);
}

std::string CStringUtils::rtrim(const std::string& s, const char* c)
{
    size_t e = s.find_last_not_of(c);
    if(e == std::string::npos)
        return s;
    return s.substr(e + 1);
}

std::string CStringUtils::join(const std::vector<std::string>& strs, const std::string& s)
{
    int n = strs.size();
    std::string restr;
    for(int i = 0; i < n - 1; ++i)
        restr = restr + strs[i] + s;
    restr += strs[n - 1];
    return restr;
}

std::string CStringUtils::lower(const std::string& s) 
{
    std::string result = s;
    for(auto& c : result)
        c = std::tolower(c);
    return result;
}


std::string CStringUtils::upper(const std::string& s) 
{
    std::string result = s;
    for(auto& c : result)
        c = std::toupper(c);
    return result;
}

int CStringUtils::hexchar2int(char c) 
{
    if( c >= '0' and c <= '9')
        return c - '0';
    else if( c >= 'A' and c <= 'Z')
        return c - 'A' + 10;
    else if( c >= 'a' and c <= 'z')
        return c - 'a' + 10;
    
    return -1;
}

std::string CStringUtils::hex(const char* data, int size)
{
    std::string result(size * 2, '0');
    for (int i = 0; i < size; ++i) {
        int c = (data[i] >> 4) bitand 0xf;
        if (c < 10) {
            result[i * 2] = c + '0';
        } else {
            result[i * 2] = (c - 10) + 'a';
        }
        c = data[i] bitand 0xf;
        if (c < 10) {
            result[i * 2 + 1] = c + '0';
        } else {
            result[i * 2 + 1] = (c - 10) + 'a';
        }
    }
    return result;
}

std::string CStringUtils::base64_encode(const std::string& src) 
{
    StringBuilder ss;
	ss.clear();

	size_t length = src.size();
	if (src.empty())
		return src;

	unsigned char parts[4];
	for (size_t i = 0; i < length; i += 3) {
		parts[0] = (src[i] & 0xfc) >> 2;
		parts[1] = ((src[i] & 0x03) << 4) | (((length > (i + 1) ? src[i + 1] : 0x00) & 0xf0) >> 4);
		parts[2] = length > (i + 1) ? (((src[i + 1] & 0x0f) << 2) |
			(((length > (i + 2) ? src[i + 2] : 0x00) & 0xc0) >> 6)) : 0x40;
		parts[3] = length > (i + 2) ? (src[i + 2] & 0x3f):0x40;

		for (int j = 0; j < 4; ++j) {
			ss << BASE64_CHARS[parts[j]];
		}
	}
	return ss.str();
}


std::string CStringUtils::base64_decode(const std::string& src) 
{
    StringBuilder ss;
	ss.clear();
	size_t length = src.size();
	if (src.empty()) {
		return src;
	}

	unsigned char parts[4];
	for (size_t i = 0; i < length; i += 4) {
		parts[0] = base64_decode_char(src[i]);
		parts[1] = length > (i + 1) ? base64_decode_char(src[i + 1]) : 64;
		parts[2] = length > (i + 2) ? base64_decode_char(src[i + 2]) : 64;
		parts[3] = length > (i + 3) ? base64_decode_char(src[i + 3]) : 64;

		ss << static_cast<unsigned char>(((parts[0] << 2) & 0xfc) | ((parts[1] >> 4) & 0x03));
		if (64 == parts[2]) {
			break;
		}
		ss << static_cast<unsigned char>(((parts[1] << 4) & 0xf0) | ((parts[2] >> 2) & 0x0f));
		if (64 == parts[3]) {
			break;
		}
		ss << static_cast<unsigned char>(((parts[2] << 6) & 0xc0) | (parts[3] & 0x3f));
	}
	return ss.str();
}


std::string CStringUtils::url_encode(const std::string& src, bool encode_slash) 
{
    StringBuilder ss;
    for (size_t i = 0; i < src.size(); ++i) {
        char c = src[i];
        if ((c >= -1 && c <= 255 && isalnum(c)) || c == '_' || c == '-' || c == '~' || c == '.'
                || (c == '/' && !encode_slash)) {
            ss << c;
        } else {
            ss << '%';
            int tmp = (c >> 4) & 0xf;
            if (tmp >= 10) {
                tmp = tmp - 10 + 'A';
            } else {
                tmp = tmp + '0';
            }
            ss << (char) tmp;
            tmp = c & 0xf;
            if (tmp >= 10) {
                tmp = tmp - 10 + 'A';
            } else {
                tmp = tmp + '0';
            }
            ss << (char) tmp;
        }
    }
    return ss.str();
}


std::string CStringUtils::url_decode(const std::string& src) 
{
    StringBuilder ss;
    for (size_t i = 0; i < src.size();) {
        if (src[i] != '%') {
            ss << src[i++];
        } else {
            ss << (char) ((hexchar2int(src[i + 1]) << 4) | hexchar2int(src[i + 2]));
            i += 3;
        }
    }
    return ss.str();
}
unsigned char CStringUtils::base64_decode_char(const unsigned char c)
{
    unsigned char result = 0;
    if (isupper(c)) {
        result = static_cast<unsigned char>(c - 'A');
    } else if (islower(c)) {
        result = static_cast<unsigned char>(c - 'a' + 26);
    } else if (isdigit(c)) {
        result = static_cast<unsigned char>(c - '0' + 52);
    } else if ('+' == c) {
        result = 62;
    } else if ('/' == c) {
        result = 63;
    } else if ('=' == c) {
        result = 64;
    } else {
        result = UCHAR_MAX;
    }
    return result;
}


std::string CStringUtils::hex(const std::string& data) 
{
    return hex(data.data(), (int)data.length());
}


bool CStringUtils::str2int(const std::string& n, int& result, int base)
{
    char* end = NULL;
    result = strtol(n.c_str(), &end, base);
    return n.c_str() + n.length() == end;
}

bool CStringUtils::str2uint64(const std::string& n, uint64_t& result, int base = 10)
{
    char* end = NULL;
    result = strtoull(n.c_str(), &end, base);
    return n.c_str() + n.length() == end;
}

bool CStringUtils::byteUnit2double(const std::string& n, double& result) 
{
    size_t pos = 0;
	for (; pos < n.size(); ++pos) {
		if (n[pos] not_eq '.' and (n[pos] < '0' or n[pos] > '9')) {
			break;
		}
	}
	char* end = NULL;
	result = strtod(n.c_str(), &end);
	if (&n[pos] not_eq end) {
		return false;
	}
	std::string unit;
	while (pos < n.size()) {
		unit.append(1, toupper(n[pos++]));
	}
	if (unit.empty() or unit == "B") {
		return true;
	}
	else if (unit == "K" or unit == "KB") {
		result *= 1024;
		return true;
	}
	else if (unit == "M" or unit == "MB") {
		result *= 1024 * 1024;
		return true;
	}
	return false;
}


bool CStringUtils::byteUnit2Int(const std::string& n, int64_t& result) 
{
    double tmp;
	if (byteUnit2double(n, tmp)) {
		result = (int64_t)tmp;
		return true;
	}
	return false;
}


END_NAMESPACE_MASS