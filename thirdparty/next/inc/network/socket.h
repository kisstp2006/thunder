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

#ifndef SOCKET_H
#define SOCKET_H

// platform detection

#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS
    #include <winsock2.h>
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <fcntl.h>
#else
    #error unsupported platform!
#endif

#include <variant.h>

#include <networkaddress.h>

class NEXT_LIBRARY_EXPORT Socket {
public:
    Socket();

    virtual ~Socket();

    virtual bool bind(const NetworkAddress &address);

    virtual bool connectToHost(const NetworkAddress &address);

    virtual void disconnectFromHost();

    void close();

    bool isValid() const;

    bool isDataAvailable() const;

    uint64_t read(char *data, uint64_t size);

    uint64_t write(const char *data, uint64_t size);

protected:
    NetworkAddress m_localAddress;
    NetworkAddress m_peerAddress;

    int32_t m_socket;

};

#endif // SOCKET_H
