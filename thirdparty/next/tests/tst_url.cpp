/*
    This file is part of Thunder Next.

    Thunder Next is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Thunder Next is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Thunder Next.  If not, see <http://www.gnu.org/licenses/>.

    Copyright: 2008-2023 Evgeniy Prikazchikov
*/

#include "tst_common.h"

#include "core/url.h"

class UrlTest : public QObject {
    Q_OBJECT
private slots:

void Parse_URL() {
    Url url("scheme://user@host:port/path/to/url?query#fragment");

    QCOMPARE(url.scheme().c_str(),  "scheme");
    QCOMPARE(url.host().c_str(),    "host");
    QCOMPARE(url.path().c_str(),    "/path/to/url");
    QCOMPARE(url.query().c_str(),   "query");
    QCOMPARE(url.fragment().c_str(),"fragment");
    QCOMPARE(url.name().c_str(),    "url");
}

void Parse_Path() {
    {
        Url url("C:\\host\\path\\to\\uri.tar.gz");

        QCOMPARE(url.path().c_str(),    "/host/path/to/uri.tar.gz");
        QCOMPARE(url.dir().c_str(),     "/host/path/to");
        QCOMPARE(url.name().c_str(),    "uri.tar.gz");
        QCOMPARE(url.baseName().c_str(),"uri");
        QCOMPARE(url.suffix().c_str(),  "tar.gz");
    }
    {
        Url url("/host/path/to/uri.tar.gz");

        QCOMPARE(url.path().c_str(),    "/host/path/to/uri.tar.gz");
        QCOMPARE(url.dir().c_str(),     "/host/path/to");
        QCOMPARE(url.name().c_str(),    "uri.tar.gz");
        QCOMPARE(url.baseName().c_str(),"uri");
        QCOMPARE(url.suffix().c_str(),  "tar.gz");
    }
}

} REGISTER(UriTest)

#include "tst_url.moc"
