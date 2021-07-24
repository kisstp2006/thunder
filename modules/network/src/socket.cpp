#include "socket.h"

#include "address.h"

Socket::Socket() {
    m_Socket = 0;
}

Socket::~Socket() {
    close();
}

bool Socket::open(uint16_t port) {
    // create socket
    m_Socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(m_Socket <= 0) {
        m_Socket = 0;
        return false;
    }

    // bind to port
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(bind(m_Socket, (const sockaddr*) &address, sizeof(sockaddr_in)) < 0) {
        close();
        return false;
    }

    // set non-blocking io
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
        int nonBlocking = 1;
        if(fcntl(m_Socket, F_SETFL, O_NONBLOCK, nonBlocking) == -1) {
            close();
            return false;
        }
#elif PLATFORM == PLATFORM_WINDOWS
        DWORD nonBlocking = 1;
        if(ioctlsocket(m_Socket, FIONBIO, &nonBlocking) != 0) {
            close();
            return false;
        }

#endif

    return true;
}

void Socket::close() {
    if(m_Socket != 0) {
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
        close(m_Socket);
#elif PLATFORM == PLATFORM_WINDOWS
        closesocket(m_Socket);
#endif
        m_Socket = 0;
    }
}

bool Socket::isOpen() const {
    return m_Socket != 0;
}

bool Socket::send(const Address &destination, const void *data, int size) {
    if(m_Socket == 0) {
        return false;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(destination.address());
    address.sin_port = htons(destination.port());

    int sent = sendto(m_Socket, (const char*)data, size, 0, (sockaddr*)&address, sizeof(sockaddr_in));

    return (sent == size);
}

int Socket::receive(Address &sender, void *data, int size) {
    if(m_Socket == 0) {
        return false;
    }

#if PLATFORM == PLATFORM_WINDOWS
    typedef int socklen_t;
#endif

    sockaddr_in from;
    socklen_t length = sizeof(from);

    int received_bytes = recvfrom(m_Socket, (char*)data, size, 0, (sockaddr*)&from, &length);

    if(received_bytes <= 0) {
        return 0;
    }

    uint32_t address = ntohl(from.sin_addr.s_addr);
    uint16_t port = ntohs(from.sin_port);

    sender = Address(address, port);

    return received_bytes;
}
