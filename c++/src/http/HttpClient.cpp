

#include "HttpClient.h"
#include "curl_easy_impl.h"


interface http_service
{
    /// a interface for each implement
public:
    HttpResponse Request()

private:
    
};



        /////////////  CurlEasyImpl ////////////////
class CurlEasyImpl final
    : public http_service
{
public:
    CurlEasyImpl();
    virtual ~CurlEasyImpl();

private:

    
};

        /////////////  HttpClient /////////////////

HttpClient::HttpClient(impl_by impl)
{
    switch(impl)
    {
    case impl_by::CURL_EASY:
        session =  std::make_shared<CurlEasyImpl>();
    }
}

HttpClient::~HttpClient()
{
}

HttpResponse HttpClient::Request(const HttpRequest &req)
{
    session->
}
