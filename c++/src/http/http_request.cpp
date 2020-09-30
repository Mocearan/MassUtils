

#include "http_request.h"

HttpRequest::HttpRequest()
{

}

HttpRequest::~HttpRequest()
{

}

HttpRequest::SetEndpoint(const std::string& endpoint)
{
    auto pos = endpoint.find("://");
    if(pos == std::string::npos) {
        _protocol = "http";
        _host = endpoint;
    }else {
        _protocol = endpoint.substr(0, pos);
        _host = endpoint.substr(pos + 3);
    }

    pos = _host.find('/');
    if(pos not_eq std::string::npos)
        _host = _host.substr(0, pos);
}


std::string HttpRequest::GenerateURL() const
{
    StringBuilder url;
    url << _protocol << "://" << _host << Mass::CStringUtils::url_encode(_uri, false);
    if(_parameters.size() > 0) {
        url << '?';

        // attach first parameter, except for the for-loop, it's meaningless except for the first time (if(is_first))
        auto it = _parameters.begin();
        url << it->first;
        if(not it->second.empty())
            url << '=' << Mass::CStringUtils::url_encode(it->second);
        ++it;

        // attach another parameters
        for (; it not_eq _parameters.end();++it) {
            url << '&';
            url << it->first;
            if(not it->second.empty())
                url << '=' << Mass::CStringUtils::url_encode(it->second);
        }
    }
}



void HttpRequest::reset() 
{
    _headers.clear();
    _uri.clear();
    _method = HttpMethod::HTTP_METHOD_INVALID;
    _files.clear();
    _data.clear();
    _parameters.clear();
    _cookies.clear();
    _timeout = 30;
    _host.clear();
    _endpoint.clear();
    _protocol.clear();
}
