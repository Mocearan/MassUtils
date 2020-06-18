

/* =============================================================================
* -> FilePath     : /Mass/MassUtils/c++/src/common/casting.cpp
* -> Author       : Mass
* -> Date         : 2020-06-18 13:35:35
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-06-18 13:35:36
* -> Description  : 
* =============================================================================*/
#include "castingutils.h"

BEGIN_NAMESPACE_MASS

bool CCastingUtils::str2int(const std::string& n, int& result, int base)
{
    char* end = NULL;
    result = strtol(n.c_str(), &end, base);
    return n.c_str() + n.length() == end;
}

bool CCastingUtils::str2uint64(const std::string& n, uint64_t& result, int base = 10)
{
    char* end = NULL;
    result = strtoull(n.c_str(), &end, base);
    return n.c_str() + n.length() == end;
}


bool CCastingUtils::byteUnit2double(const std::string& n, double& result) 
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


bool CCastingUtils::byteUnit2Int(const std::string& n, int64_t& result) 
{
    double tmp;
	if (byteUnit2double(n, tmp)) {
		result = (int64_t)tmp;
		return true;
	}
	return false;
}

std::vector<byte> CCastingUtils::intTobyte(int num)
{
	return  std::vector<byte>{
		(byte)((num>>24) bitand 0xff)
		,(byte)((num>>16) bitand 0xff)
		,(byte)((num>>8) bitand 0xff)
		,(byte)(num bitand 0xff)
	};
}

int CCastingUtils::Byte2int(const byte arr[], size_t n)
{
	return ( arr[0] bitand 0xff) << 24 
		bitor (arr[1] bitand 0xff)<<16 
		bitor (arr[2] bitand 0xff)<<8 
		bitor (arr[3] bitand 0xff);
}

std::string CCastingUtils::bytesToHexString(const byte src[], size_t n) 
{
	StringBuilder stringBuilder;
	if (src == NULL || n <= 0) {
		return NULL;
	}
	for (int i = 0; i < n; i++) {
		std::string hv = intype2hexstr<int>(src[i] bitand 0xFF);
		if (hv.length() < 2) {
			stringBuilder << 0;
		}
		stringBuilder << hv;
	}
	return stringBuilder.str();
}

std::string CCastingUtils::BytesHexString(const byte b[], size_t n) 
{
	std::string ret = "";
	for (int i = 0; i < n; i++) {
		std::string hex = intype2hexstr(b[i] & 0xFF);
		if (hex.length() == 1) {
			hex = '0' + hex;
		}
		ret += CStringUtils::upper(hex);
	}
	return ret;
}

END_NAMESPACE_MASS