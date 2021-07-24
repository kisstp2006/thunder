#ifndef CONNECTION_H
#define CONNECTION_H

#include <cstdint>
#include <memory>

#include "address.h"
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

    void connect(const Address &address);

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
        m_State = Disconnected;
        m_TimeoutAccumulator = 0.0f;
        m_Address = Address();
    }

    enum State {
        Disconnected,
        Listening,
        Connecting,
        ConnectFail,
        Connected
    };

    uint32_t m_ProtocolId;
    float m_Timeout;
    float m_TimeoutAccumulator;

    bool m_Running;
    Mode m_Mode;
    State m_State;
    Socket m_Socket;
    Address m_Address;
};

#endif // CONNECTION_H
