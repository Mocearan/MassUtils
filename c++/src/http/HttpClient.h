
/* =============================================================================
* -> FilePath     : /WeTalkServer2/src/base/http/HttpClient.h
* -> Author       : Mass
* -> Date         : 2020-08-19 15:08:02
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-08-19 15:08:02
* -> Description  : 
                a simple http client base on curl easy interface.
                api like python requests.
                use front pattern to provide the freedom of the underlying implementation.
=============================================================================*/

#ifndef __MASSUTILS_HTTP_HTTPCLIENT_H__
#define __MASSUTILS_HTTP_HTTPCLIENT_H_

#include <curl/curl.h>

#include <string>

class HttpRequest;
class HttpResponse;
class http_service;

class HttpClient final
{
public:
    /// http client implement type definition

    enum class impl_by : char
    {
        CURL_EASY = 0,
        CURL_MULTI, // not impl
        ASIO, // not impl
        SOCKET  // not imple
    };

public:
    HttpClient(impl_by impl = impl_by::CURL_EASY);
    ~HttpClient();
    
public:
    HttpResponse Request(const HttpRequest &req);

    HttpResponse Get(const HttpRequest &req);

    HttpResponse Options(const HttpRequest &req);

    HttpResponse Head(const HttpRequest &req);

    HttpResponse Post(const HttpRequest &req);

    HttpResponse Put(const HttpRequest &req);

    HttpResponse Patch(const HttpRequestU &req);

    HttpResponse Delete(const HttpRequest &req);

private:
    std::shared_ptr<http_service> session;
};

#endif //__MASSUTILS_HTTP_HTTPCLIENT_H_ 