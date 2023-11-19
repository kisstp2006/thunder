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

#include "core/url.h"

/*!
    \class Url
    \brief Url class provides an interface for working with Url's.
    \since Next 1.0
    \inmodule Core
*/

Url::Url() {

}

Url::Url(const string &url) :
        m_url(url) {

    PROFILE_FUNCTION();
    replace(m_url.begin(), m_url.end(), '\\', '/');
    regex_match(m_url, m_result, regex("^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?"));
}

/*!
    Returns the scheme of the Url. If an empty string is returned, this means the scheme is undefined and the Url is then relative.
*/
string Url::scheme() const {
    PROFILE_FUNCTION();
    return m_result[2].str();
}
/*!
    Returns the host of the Url if it is defined; otherwise an empty string is returned.
*/
string Url::host() const {
    PROFILE_FUNCTION();
    return m_result[4];
}
/*!
    Returns the path of the Url.
*/
string Url::path() const {
    PROFILE_FUNCTION();
    return m_result[5];
}
/*!
    Returns the query string of the Url if there's a query string, or an empty result if not.
*/
string Url::query() const {
    PROFILE_FUNCTION();
    return m_result[7];
}
/*!
    Returns the fragment of the Url.
*/
string Url::fragment() const {
    PROFILE_FUNCTION();
    return m_result[9];
}
/*!
    Returns a directory of Url path.
*/
string Url::dir() const {
    PROFILE_FUNCTION();
    string str = path();
    size_t found = str.rfind('/');
    if(found != string::npos) {
        str.replace(found, str.length(), "");
    }
    return str;
}
/*!
    Returns a file name in the Url path.
*/
string Url::name() const {
    PROFILE_FUNCTION();
    string str = path();
    size_t found = str.rfind('/');
    if(found != string::npos) {
        str.replace(0, found + 1, "");
    }
    return str;
}
/*!
    Returns a base name of file in the Url path.
*/
string Url::baseName() const {
    PROFILE_FUNCTION();
    string str = name();
    size_t found = str.find('.');
    if(found != string::npos) {
        str.replace(found, str.length(), "");
    }
    return str;
}
/*!
    Returns a file suffix in the Url path.
*/
string Url::suffix() const {
    PROFILE_FUNCTION();
    string str = name();
    size_t found = str.find('.');
    if(found != string::npos) {
        str.replace(0, found + 1, "");
    }
    return str;
}
