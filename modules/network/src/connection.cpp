#include "connection.h"

Connection::Connection(uint32_t protocolId, float timeout) {
    m_ProtocolId = protocolId;
    m_Timeout = timeout;
    m_Mode = None;
    m_Running = false;
    clearData();
}

Connection::~Connection() {
    if(isRunning()) {
        stop();
    }
}

bool Connection::start(int port) {
    if(!m_Socket.open(port)) {
        return false;
    }
    m_Running = true;
    onStart();
    return true;
}

void Connection::stop() {
    bool connected = isConnected();
    clearData();
    m_Socket.close();
    m_Running = false;
    if(connected) {
        onDisconnect();
    }
    onStop();
}

void Connection::listen() {
    bool connected = isConnected();
    clearData();
    if(connected) {
        onDisconnect();
    }
    m_Mode = Server;
    m_State = Listening;
}

void Connection::connect(const Address &address) {
    bool connected = isConnected();
    clearData();
    if(connected) {
        onDisconnect();
    }
    m_Mode = Client;
    m_State = Connecting;
    m_Address = address;
}

bool Connection::isRunning() const {
    return m_Running;
}

bool Connection::isConnecting() const {
    return m_State == Connecting;
}

bool Connection::isConnectFailed() const {
    return m_State == ConnectFail;
}

bool Connection::isConnected() const {
    return m_State == Connected;
}

bool Connection::isListening() const {
    return m_State == Listening;
}

Connection::Mode Connection::mode() const {
    return m_Mode;
}

void Connection::update(float dt) {
    m_TimeoutAccumulator += dt;
    if(m_TimeoutAccumulator > m_Timeout) {
        if(m_State == Connecting) {
            clearData();
            m_State  = ConnectFail;
            onDisconnect();
        }
        else if(m_State == Connected) {
            clearData();
            if(m_State == Connecting) {
                m_State  = ConnectFail;
            }
            onDisconnect();
        }
    }
}

bool Connection::sendPacket(const uint8_t data[], int size) {
    if(m_Address.address() == 0) {
        return false;
    }
    uint8_t *packet = new uint8_t[size + 4];
    packet[0] = (uint8_t) ( m_ProtocolId >> 24);
    packet[1] = (uint8_t) ((m_ProtocolId >> 16) & 0xFF);
    packet[2] = (uint8_t) ((m_ProtocolId >> 8) & 0xFF);
    packet[3] = (uint8_t) ((m_ProtocolId) & 0xFF);

    memcpy(&packet[4], data, size);

    bool result = m_Socket.send(m_Address, packet, size + 4);

    delete []packet;

    return result;
}

int Connection::receivePacket(uint8_t data[], int size) {
    Address sender;
    uint8_t *packet = new uint8_t[size + 4];
    int bytesRead   = m_Socket.receive(sender, packet, size + 4);
    int result      = 0;
    if(bytesRead >= 4) {
        if(packet[0] == (uint8_t) ( m_ProtocolId >> 24) &&
           packet[1] == (uint8_t) ((m_ProtocolId >> 16) & 0xFF) &&
           packet[2] == (uint8_t) ((m_ProtocolId >> 8) & 0xFF) &&
           packet[3] == (uint8_t) ((m_ProtocolId) & 0xFF)) {

            if(m_Mode == Server && !isConnected()) {
                m_State      = Connected;
                m_Address    = sender;
                onConnect();
            }
            if(sender == m_Address) {
                if(m_Mode == Client && m_State == Connecting) {
                    m_State  = Connected;
                    onConnect();
                }
                m_TimeoutAccumulator = 0.0f;
                memcpy(data, &packet[4], bytesRead - 4);
                result  = bytesRead - 4;
            }
        }
    }

    delete []packet;

    return result;
}

int Connection::headerSize() const {
    return 4;
}
