

/* =============================================================================
* -> FilePath     : /MassUtils/c++/src/common/configfileutils.hpp
* -> Author       : Mass
* -> Date         : 2020-06-19 21:56:34
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-06-19 21:56:35
* -> Description  : 
* =============================================================================*/

#ifndef __MASS_COMMON_CONFIGFILEUTILS_HPP__
#define __MASS_COMMON_CONFIGFILEUTILS_HPP__

#include "singleton.h"
#include "simplelogutils.hpp"

#include <cstring>

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
    void _parse_line(char *line);
    char *trim(char *name);

private:
    bool m_bLoaded = false;
    StringMap m_config_map;
    std::string m_config_file;
};

CConfigFileReader::CConfigFileReader(const std::string &filename)
{
    _load_file(filename);
}

CConfigFileReader::~CConfigFileReader() {}

int CConfigFileReader::SetConfigValue(const std::string &name, const std::string &value)
{
    if (not m_bLoaded)
        return -1;

    auto it = m_config_map.find(name);
    if (it not_eq m_config_map.end())
        it->second = value;
    else
        m_config_map.insert({name, value});

    return _write_file();
}

#include <iostream>

std::string CConfigFileReader::GetConfigName(const std::string &name)
{
    string value;

    if (not m_bLoaded)
        return value;

    auto it = m_config_map.find(name);
    if (it not_eq m_config_map.end())
        value = it->second.data();

    return value;
}

void CConfigFileReader::_parse_line(char *line)
{
    char *p = strchr(line, '=');
    if (p == nullptr)
        return;

    *p = 0;
    char *key = trim(line);
    char *value = trim(p + 1);
    if (key and value)
        m_config_map.insert({key, value});
}

int CConfigFileReader::_write_file(const std::string &filename)
{
    FILE *fp = nullptr;

    if ("" == filename)
        fp = fopen(m_config_file.data(), "w");
    else
        fp = fopen(filename.data(), "w");

    if (fp == nullptr)
        return -1;

    char szPaire[128]{'\0'};
    for (const auto &it : m_config_map)
    {
        bzero(szPaire, sizeof(szPaire));
        snprintf(szPaire, sizeof(szPaire), "%s=%s\n", it.first.data(), it.second.data());
        auto ret = fwrite(szPaire, strlen(szPaire), 1, fp);
        if (ret not_eq 1)
        {
            fclose(fp);
            return -1;
        }
    }
    fclose(fp);
    return 0;
}

void CConfigFileReader::_load_file(const std::string &filename)
{
    m_config_file.clear();
    m_config_file.append(filename);

    auto fp = fopen(filename.data(), "r");
    if (not fp)
    {
        log(loglevel::ERROR, "can not open %s, errno = %d", filename.data(), errno);
        return;
    }

    char buf[256]{'\0'};
    for (;;)
    {
        char *p = fgets(buf, 256, fp);
        if (not p)
            break;

        size_t len = strlen(buf);

        if (buf[len - 1] == '\n')
            buf[len - 1] = '\0';

        char *ch = strchr(buf, '#');
        if (ch)
            *ch = 0;

        if (strlen(buf) == 0)
            continue;

        _parse_line(buf);
    }

    fclose(fp);
    m_bLoaded = true;
}

char *CConfigFileReader::trim(char *name)
{
    // remove starting space or tab
    char *start_pos = name;
    while ((*start_pos == ' ') or (*start_pos == '\t'))
        start_pos++;

    if (strlen(start_pos) == 0)
        return NULL;

    // remove ending space or tab
    char *end_pos = name + strlen(name) - 1;
    while ((*end_pos == ' ') or (*end_pos == '\t'))
    {
        *end_pos = 0;
        end_pos--;
    }

    int len = (int)(end_pos - start_pos) + 1;
    if (len <= 0)
        return NULL;

    return start_pos;
}

END_NAMESPACE_MASS

#endif // __MASS_COMMON_CONFIGFILEUTILS_HPP__