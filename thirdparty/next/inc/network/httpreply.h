/*
    This file is part of Thunder Next.

    Thunder Next is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    Thunder Next is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Thunder Next.  If not, see <http://www.gnu.org/licenses/>.

    Copyright: 2008-2023 Evgeniy Prikazchikov
*/

#ifndef HTTPREPLY_H
#define HTTPREPLY_H

#include <objectsystem.h>

#include <httprequest.h>
#include <tcpsocket.h>

class HttpRequest;

class NEXT_LIBRARY_EXPORT HttpReply {
public:
    enum ReplyState {
        Initial = 0,
        Connecting,
        SendingHeader,
        ReadingCode,
        ReadingHeader,
        ReadingContent,
        Done
    };

public:
    HttpReply();

    int errorCode() const;

    void setRequest(const string &operation, const HttpRequest &request);

protected:
    void readAnswer();

protected:
    HttpRequest m_request;

    TcpSocket m_socket;

    ByteArray m_content;

    map<string, string> m_fields;

    int m_code;

    int m_state;

    size_t m_contentSize;

};

#endif // HTTPREPLY_H
