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

#include "udpsocket.h"

#include "networkaddress.h"

bool UdpSocket::bind(const NetworkAddress &address) {
    m_socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    return Socket::bind(address);
}

uint64_t UdpSocket::readDatagram(int8_t *data, uint64_t maxSize, NetworkAddress *address) {
    if(m_socket == 0) {
        return 0;
    }

    uint64_t receivedBytes = 0;

    if(address) {
#if PLATFORM == PLATFORM_WINDOWS
        typedef int socklen_t;
#endif
        sockaddr_in from;
        socklen_t length = sizeof(from);

        receivedBytes = ::recvfrom(m_socket, (char*)data, maxSize, 0, (sockaddr*)&from, &length);

        *address = NetworkAddress(ntohl(from.sin_addr.s_addr), ntohs(from.sin_port));
    } else {
        receivedBytes = ::recvfrom(m_socket, (char*)data, maxSize, 0, nullptr, nullptr);
    }

    return MAX(receivedBytes, 0);
}

uint64_t UdpSocket::writeDatagram(int8_t *data, uint64_t size, const NetworkAddress &address) {
    if(m_socket == 0) {
        return false;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(address.toIPv4Adress());
    addr.sin_port = htons(address.port());

    uint64_t sent = sendto(m_socket, (const char*)data, size, 0, (sockaddr*)&address, sizeof(sockaddr_in));

    return sent;
}
