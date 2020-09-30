
/* =============================================================================
* -> FilePath     : /WeTalkServer2/src/base/http/http_response.h
* -> Author       : Mass
* -> Date         : 2020-09-02 10:03:16
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-09-02 10:03:47
* -> Description  : 
* =============================================================================*/

#ifndef __MASSUTILS_HTTP_HTTPRESPONSE_H__
#define __MASSUTILS_HTTP_HTTPRESPONSE_H__

class HttpResponse
{
public:
    HttpResponse();
    ~HttpResponse();

    

private:
    int parse_status_line(const std::string &status_line);
    int parse_header_line(const std::string &header_line, std::string &key, std::string &value);

private:
    /* interger code of responded HTTP status  */
    int _status_code;

    /* map of response headers */
    StringMap _headers;

    /* final url location of response */
    std::string _url;

    /* http protocol version */
    std::string _version;

    /* http status massage */
    std::string _reson;

    /* http error message */
    StringBuilder _error_stream;

    bool _has_recv_status_line = false;
};

#endif //__MASSUTILS_HTTP_HTTPRESPONSE_H__ 