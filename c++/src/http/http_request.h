
/* =============================================================================
* -> FilePath     : /WeTalkServer2/src/base/http/http_request.h
* -> Author       : Mass
* -> Date         : 2020-08-31 16:47:42
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-08-31 16:47:46
* -> Description  : 
* =============================================================================*/

#ifndef __MASSUTILS_HTTP_HTTPREQUEST_H__
#define __MASSUTILS_HTTP_HTTPREQUEST_H__

#include "common/stringutils.h"

#include <vector>

enum class HttpMethod : char
{
    HTTP_METHOD_INVALID = 0,
    HTTP_METHOD_GET,
    HTTP_METHOD_PUT,
    HTTP_METHOD_POST,
    HTTP_METHOD_HEAD,
    HTTP_METHOD_DELETE,
    HTTP_METHOD_OPTIONS,
    HTTP_METHOD_PATCH
};

static inline const char *method_str(HttpMethod method) {
    switch (method) {
    case HTTP_METHOD_PUT:
        return "PUT";
    case HTTP_METHOD_GET:
        return "GET";
    case HTTP_METHOD_POST:
        return "POST";
    case HTTP_METHOD_HEAD:
        return "HEAD";
    case HTTP_METHOD_DELETE:
        return "DELETE";
    default:
        return "UNKNOWN";
    }
}

class HttpRequest
{
private:
    //using Auth = tuple<std::string, std::string>;

public:
    HttpRequest();
    ~HttpRequest();

    HttpRequest(HttpMethod method, );

public:
    void SetMethod(HttpMethod method) { _method = method; }

    HttpMethod Method() const { return _method; }

    void AddHeader(const std::string& key, const std::string& value)
    {
        _headers.insert({key, value});
    }

    void AddHeader(const std::string& key, int64_t value)
    {
        _headers.insert({key, Mass::StringUtils::to_string(value)});
    }

    const StringMap& Headers () const {return _headers;}

    void AddParameter(const std::string& key, const std::string& value)
    {
        _parameters.insert({key, value});
    }

    void AddParameter(const std::string &key, int64_t value)
    {
        _parameters.insert({keys, Mass::CStringUtils::to_string(value)});
    }

    const StringMap& Paramters() const {return _parameters;}

    void AddCookie(const std::string& key, const std::string& value)
    {
        _cookies.insert({key, value});
    }

    const StringMap& Cookies() const {return _cookies;}

    void AddData(const std::string& key, const std::string& value)
    {
        _data.insert({key, value});
    }

    const StringMap& Data() const {return _data;}

    void AddFiles(const std::string& file)
    {
        _files.push_back(file);
    }

    const std::vector<std::string>& Files() const {return _files;}

    void SetTimeout(int timeout) { _timeout = timeout; }

    int Timeout() const { return _timeout; }

    void SetUri(const std::string &uri) { _uri = uri; }

    const std::string &uri() const { return _uri; }

    void SetEndpoint(const std::string &endpoint);

    const std::string &Host() const { return _host; }

    std::string GenerateURL() const;

    // reset to state before build that can rebuild again
    void reset();

    std::string ToString() 
    { 
        return std::string(method_str(_method)) + " " + GenerateURL(); 
    }

private:

    /* a string k-v map to each send and receive. */
    StringMap _headers;

    /* an authentication handler or a tuple. */
    //Auth auth_;

    /* client proxies */
    //StringMap proxies;

    /* HTTP method */
    HttpMethod _method = HttpMethod::HTTP_METHOD_INVALID;

    std::string _uri;

    /* files {filename} for multipart upload*/
    std::vector<std::string> _files;

    /* the body to attach to the request. */
    StringMap _data;

    /* json for the body to attach to the request(if files or data is not specified). */
    // Json _json;

    /* URL parameters to append to the URL. */
    StringMap _parameters;

    /* cookies to attach to this request */
    StringMap _cookies;

    /* seconds, the wait time for each request. */
    int _timeout = 30;

    /*  */
    std::string _host;

    /* ip and port */
    std::string _endpoint;

    /* http protocol version */
    std::string _protocol;
};

#endif //__MASSUTILS_HTTP_HTTPREQUEST_H__ 