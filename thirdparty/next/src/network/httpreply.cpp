#include "httpreply.h"

#include <memory>

HttpReply::HttpReply() :
        m_code(0),
        m_state(ReplyState::Initial),
        m_contentSize(0) {

}

int HttpReply::errorCode() const {
    return m_code;
}

void HttpReply::setRequest(const string &operation, const HttpRequest &request) {
    m_request = request;

    string host(m_request.url().host());
    string scheme(m_request.url().scheme());
    string path(m_request.url().path());
    string header(operation + " " + (path.empty() ? "/" : path) + " HTTP/1.1\r\n");

    header += m_request.header();

    if(scheme == "http") {
        m_state = ReplyState::Connecting;
        if(m_socket.connectToHost(NetworkAddress(host, 80))) {
            // Prepare HTTP request
            m_state = ReplyState::SendingHeader;
            if(m_socket.write(header.c_str(), header.size()) == header.size()) {
                m_content.clear();

                m_state = ReplyState::ReadingCode;
                readAnswer();
            }
        }
    } else if(scheme == "https") {
        // HTTPS is unsupported yet
    }
}

list<string> split(const string &source, const string &delimiter) {
    size_t posStart = 0, posEnd, delimLen = delimiter.length();
    string token;

    list<string> result;
    while((posEnd = source.find(delimiter, posStart)) != string::npos) {
        token = source.substr(posStart, posEnd - posStart);
        posStart = posEnd + delimLen;
        result.push_back(token);
    }

    result.push_back(source.substr(posStart));

    return result;
}

void HttpReply::readAnswer() {
    char sub[1024];

    bool flag = true;
    string result;
    do {
        uint64_t size = m_socket.read(sub, sizeof(sub));
        if(size > 0) {
            if(size < sizeof(sub)) {
                sub[size] = '\0';
            }

            for(auto &it : split(sub, "\r\n")) {
                switch(m_state) {
                case ReplyState::ReadingCode: {
                    auto tokens = split(it, " ");
                    if(tokens.size() > 2) {
                        auto token = next(tokens.begin(), 1);
                        if(!token->empty()) {
                           char *end;
                           m_code = strtol(token->c_str(), &end, 10);
                        }
                    }
                    m_state = ReplyState::ReadingHeader;
                } break;
                case ReplyState::ReadingHeader: {
                    bool end = it.empty();
                    if(!end) {
                        auto field = split(it, ": ");

                        m_fields[field.front()] = field.back();
                    } else {
                        auto it = m_fields.find("Content-Length");
                        if(it != m_fields.end()) {
                            char *end;
                            m_contentSize = strtol(it->second.c_str(), &end, 10);
                        }
                        m_state = ReplyState::ReadingContent;
                    }
                } break;
                case ReplyState::ReadingContent: {
                    m_content.append(it);
                } break;
                default: break;
                }
            }

            flag = m_socket.isDataAvailable();
        } else {
            flag = false;
        }

    } while(flag);
}
