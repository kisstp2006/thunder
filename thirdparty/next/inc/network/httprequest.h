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

#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <url.h>

#include <variant.h>

class HttpReply;

class NEXT_LIBRARY_EXPORT HttpRequest {

public:
    HttpRequest();

    HttpRequest(const Url &url);

    Url url() const;

    HttpReply *get();

    string header() const;

private:
    Url m_url;

    string m_operation;

    list<pair<string, string>> m_headers;

};

#endif // HTTPREQUEST_H
