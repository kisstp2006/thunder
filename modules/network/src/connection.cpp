#include "connection.h"

Connection::Connection(uint32_t protocolId, float timeout) {
    m_protocolId = protocolId;
    m_timeout = timeout;
    m_mode = None;
    m_running = false;
    clearData();
}

Connection::~Connection() {
    if(isRunning()) {
        stop();
    }
}

bool Connection::start(int port) {
    //if(!m_socket.open(port)) {
    //    return false;
    //}
    m_running = true;
    onStart();
    return true;
}

void Connection::stop() {
    bool connected = isConnected();
    clearData();
    m_socket.close();
    m_running = false;
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
    m_mode = Server;
    m_state = Listening;
}

void Connection::connect(const NetworkAddress &address) {
    bool connected = isConnected();
    clearData();
    if(connected) {
        onDisconnect();
    }
    m_mode = Client;
    m_state = Connecting;
    m_address = address;
}

bool Connection::isRunning() const {
    return m_running;
}

bool Connection::isConnecting() const {
    return m_state == Connecting;
}

bool Connection::isConnectFailed() const {
    return m_state == ConnectFail;
}

bool Connection::isConnected() const {
    return m_state == Connected;
}

bool Connection::isListening() const {
    return m_state == Listening;
}

Connection::Mode Connection::mode() const {
    return m_mode;
}

void Connection::update(float dt) {
    m_timeoutAccumulator += dt;
    if(m_timeoutAccumulator > m_timeout) {
        if(m_state == Connecting) {
            clearData();
            m_state  = ConnectFail;
            onDisconnect();
        }
        else if(m_state == Connected) {
            clearData();
            if(m_state == Connecting) {
                m_state  = ConnectFail;
            }
            onDisconnect();
        }
    }
}

bool Connection::sendPacket(const uint8_t data[], int size) {
    if(m_address.toIPv4Adress() == 0) {
        return false;
    }
    uint8_t *packet = new uint8_t[size + 4];
    packet[0] = (uint8_t) ( m_protocolId >> 24);
    packet[1] = (uint8_t) ((m_protocolId >> 16) & 0xFF);
    packet[2] = (uint8_t) ((m_protocolId >> 8) & 0xFF);
    packet[3] = (uint8_t) ((m_protocolId) & 0xFF);

    memcpy(&packet[4], data, size);

    bool result = true;//m_socket.send(m_address, packet, size + 4);

    delete []packet;

    return result;
}

int Connection::receivePacket(uint8_t data[], int size) {
    NetworkAddress sender;
    uint8_t *packet = new uint8_t[size + 4];
    int bytesRead = 0;//m_socket.receive(sender, packet, size + 4);
    int result = 0;
    if(bytesRead >= 4) {
        if(packet[0] == (uint8_t) ( m_protocolId >> 24) &&
           packet[1] == (uint8_t) ((m_protocolId >> 16) & 0xFF) &&
           packet[2] == (uint8_t) ((m_protocolId >> 8) & 0xFF) &&
           packet[3] == (uint8_t) ((m_protocolId) & 0xFF)) {

            if(m_mode == Server && !isConnected()) {
                m_state = Connected;
                m_address = sender;
                onConnect();
            }
            if(sender == m_address) {
                if(m_mode == Client && m_state == Connecting) {
                    m_state = Connected;
                    onConnect();
                }
                m_timeoutAccumulator = 0.0f;
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
