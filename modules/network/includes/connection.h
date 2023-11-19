#ifndef CONNECTION_H
#define CONNECTION_H

#include <cstdint>
#include <memory>

#include <networkaddress.h>
#include "socket.h"

class Connection {
public:
    enum Mode {
        None,
        Client,
        Server
    };

    Connection(uint32_t protocolId, float timeout);

    virtual ~Connection();

    bool start(int port);

    void stop();

    void listen();

    void connect(const NetworkAddress &address);

    bool isRunning() const;

    bool isConnecting() const;

    bool isConnectFailed() const;

    bool isConnected() const;

    bool isListening() const;

    Mode mode() const;

    virtual void update(float dt);

    virtual bool sendPacket(const uint8_t data[], int size);

    virtual int receivePacket(uint8_t data[], int size);

    int headerSize() const;

protected:
    virtual void onStart() {}
    virtual void onStop() {}
    virtual void onConnect() {}
    virtual void onDisconnect() {}

private:
    void clearData() {
        m_state = Disconnected;
        m_timeoutAccumulator = 0.0f;
        m_address = NetworkAddress();
    }

    enum State {
        Disconnected,
        Listening,
        Connecting,
        ConnectFail,
        Connected
    };

    Socket m_socket;
    NetworkAddress m_address;

    Mode m_mode;
    State m_state;

    uint32_t m_protocolId;

    float m_timeout;
    float m_timeoutAccumulator;

    bool m_running;

};

#endif // CONNECTION_H
