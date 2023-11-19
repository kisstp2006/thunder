#include "httprequest.h"

#include "httpreply.h"

HttpRequest::HttpRequest() {

}

HttpRequest::HttpRequest(const Url &url) :
        m_url(url) {

    m_headers.push_back(make_pair("Host", m_url.host()));
    m_headers.push_back(make_pair("Connection", "keep-alive"));
}

Url HttpRequest::url() const {
    return m_url;
}

string HttpRequest::header() const {
    string result;

    for(auto &it : m_headers) {
        result += it.first + ": " + it.second + "\r\n";
    }

    result += "\r\n";

    return result;
}

HttpReply *HttpRequest::get() {
    HttpReply *result = new HttpReply;
    result->setRequest("GET", *this);

    return result;
}

HttpReply *HttpRequest::post(const ByteArray &data) {
    HttpReply *result = new HttpReply;
    result->setRequest("POST", *this);

    return result;
}
