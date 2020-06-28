
/* =============================================================================
* -> FilePath     : /WeTalkServer2/src/base/utils/ConfigFileReader.h
* -> Author       : Mass
* -> Date         : 2020-06-19 18:14:21
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-06-19 18:14:22
* -> Description  : 
        a cache pool implement base redis
* =============================================================================*/

#ifndef __MASSUTILS_COMMON_CONFIGFILEREADER_H__
#define __MASSUTILS_COMMON_CONFIGFUKEREADER_H__

#include "singleton.h"

#include <map>

BEGIN_NAMESPACE_MASS

class CConfigFileReader final
    : public Mass::Singleton<CConfigFileReader>
{
    friend class Mass::Singleton<CConfigFileReader>;
    CConfigFileReader(const std::string &filename);

public:
    virtual ~CConfigFileReader();

    std::string GetConfigName(const std::string &name);
    int SetConfigValue(const std::string &name, const std::string &value);

private:
    void _load_file(const std::string &filename);
    int _write_file(const std::string &filename = "");
    void _parse_line(char* line);
    char* trim(char* name);

private:
    bool m_bLoaded = false;
    std::map<std::string, std::string> m_config_map;
    std::string m_config_file;
};

END_NAMESPACE_MASS

#endif // __MASSUTILS_COMMON_CONFIGFUKEREADER_H__