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

#include <cstdint>

class Address;

class Socket {
public:
    Socket();

    ~Socket();

    bool open(uint16_t port);

    void close();

    bool isOpen() const;

    bool send(const Address &destination, const void *data, int size);

    int receive(Address &sender, void *data, int size);

private:
    int m_Socket;

};

#endif // SOCKET_H
