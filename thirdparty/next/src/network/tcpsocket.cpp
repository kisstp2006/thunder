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

#include "tcpsocket.h"

#include "networkaddress.h"

TcpSocket::TcpSocket() :
        Socket() {

    m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

bool TcpSocket::connectToHost(const NetworkAddress &address) {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = address.toIPv4Adress();
    addr.sin_port = htons(address.port());

    return ::connect(m_socket, (struct sockaddr*)&addr, sizeof(addr)) == 0;
}

void TcpSocket::disconnectFromHost() {
    ::closesocket(m_socket);
}
