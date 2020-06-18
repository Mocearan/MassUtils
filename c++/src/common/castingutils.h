

/* =============================================================================
* -> FilePath     : /Mass/MassUtils/c++/src/common/casting.h
* -> Author       : Mass
* -> Date         : 2020-06-18 13:29:27
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-06-18 13:29:28
* -> Description  : 
* =============================================================================*/

#ifndef __MASSUTILS_COMMON_CASTING_H__
#define __MASSUTILS_COMMON_CASTING_H__

#include "noninstantiable.h"
#include "stringutils.h"

#include <vector>
#include <iomanip>

BEGIN_NAMESPACE_MASS

class CCastingUtils
    : public noninstantiable
{
public:
    static bool str2int(const std::string& n, int& result, int base = 10);
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
        
        StringBuilder vStream;
        for(std::size_t i = 0 ; i < sizeof(Inttype) ; ++i)
            vStream << "\\x"
                    << std::right << std::setfill('0') << std::setw(2) << std::hex
                    << ((a >> i*4) & 0xFF);

        return vStream.str();
    }
    static bool byteUnit2double(const std::string& n, double& result);
    static bool byteUnit2Int(const std::string& n, int64_t& result);

    static std::vector<byte> intTobyte(int num);
    static int Byte2int(const byte arr[], size_t n);

    static std::string bytesToHexString(const byte src[], size_t n);

    static std::string BytesHexString(const byte b[], size_t n);

};

END_NAMESPACE_MASS

#endif // __MASSUTILS_COMMON_CASTING_H__