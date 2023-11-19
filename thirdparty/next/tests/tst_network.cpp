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

#include <httprequest.h>
#include <url.h>

class NetworkTest : public QObject {
    Q_OBJECT
private slots:
    void Basic_TcpSocket_Check() {

    }

    void Basic_Http_Check() {
        HttpReply *reply = HttpRequest(Url("http://thunderengine.org/")).get();
        if(reply) {

        }
    }

} REGISTER(NetworkTest)

#include "tst_network.moc"
