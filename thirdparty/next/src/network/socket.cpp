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

#include "socket.h"

#include "networkaddress.h"

Socket::Socket() :
        m_socket(0) {

#if PLATFORM == PLATFORM_WINDOWS
    static bool init = false;
    if(!init) {
        WSADATA wsaData;
        init = (WSAStartup(WINSOCK_VERSION, &wsaData) == 0);
    }
#endif
}

Socket::~Socket() {
    close();
}

bool Socket::bind(const NetworkAddress &address) {
    if(m_socket <= 0) {
        m_socket = 0;
        return false;
    }

    m_localAddress = address;

    // bind to port
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(m_localAddress.toIPv4Adress());
    addr.sin_port = htons(m_localAddress.port());

    if(::bind(m_socket, (const sockaddr*)&address, sizeof(sockaddr_in)) < 0) {
        close();
        return false;
    }

    // set non-blocking io
//#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
//    int nonBlocking = 1;
//    if(fcntl(m_socket, F_SETFL, O_NONBLOCK, nonBlocking) == -1) {
//        close();
//        return false;
//    }
//#elif PLATFORM == PLATFORM_WINDOWS
//    DWORD nonBlocking = 1;
//    if(ioctlsocket(m_socket, FIONBIO, &nonBlocking) != 0) {
//        close();
//        return false;
//    }
//#endif

    return true;
}

bool Socket::connectToHost(const NetworkAddress &address) {
    m_peerAddress = address;

    return true;
}

void Socket::disconnectFromHost() {

}

void Socket::close() {
    if(m_socket != 0) {
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
        ::close(m_socket);
#elif PLATFORM == PLATFORM_WINDOWS
        ::closesocket(m_socket);
#endif
        m_socket = 0;
    }
}

bool Socket::isValid() const {
    return m_socket != 0;
}

bool Socket::isDataAvailable() const {
    u_long flag = 0;
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
    ::ioctl(m_socket, FIONREAD, &flag);
#elif PLATFORM == PLATFORM_WINDOWS
    ::ioctlsocket(m_socket, FIONREAD, &flag);
#endif

    return flag > 0;
}

uint64_t Socket::read(char *data, uint64_t size) {
    return ::recv(m_socket, data, size, 0);
}

uint64_t Socket::write(const char *data, uint64_t size) {
    return ::send(m_socket, data, size, 0);
}
