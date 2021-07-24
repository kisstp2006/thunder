#include "reliableconnection.h"

ReliableConnection::ReliableConnection(uint32_t protocolId, float timeout, uint32_t maxSequence)
        : Connection(protocolId, timeout),
        m_ReliabilitySystem(maxSequence) {
    clearData();
}

ReliableConnection::~ReliableConnection() {
    if(isRunning()) {
        stop();
    }
}

bool ReliableConnection::sendPacket(const uint8_t data[], int size) {
    const int header = 12;
    uint8_t *packet = new uint8_t[header+size];

    uint32_t seq = m_ReliabilitySystem.localSequence();
    uint32_t ack = m_ReliabilitySystem.remoteSequence();
    uint32_t ack_bits = m_ReliabilitySystem.generateAckBits();
    writeHeader(packet, seq, ack, ack_bits);
    memcpy(packet + header, data, size);
    if(!Connection::sendPacket(packet, size + header)) {
        delete []packet;
        return false;
    }
    delete []packet;

    m_ReliabilitySystem.packetSent(size);
    return true;
}

int ReliableConnection::receivePacket(uint8_t data[], int size) {
    const int header = 12;
    if(size <= header) {
        return false;
    }
    uint8_t *packet = new uint8_t[header+size];
    int received_bytes = Connection::receivePacket(packet, size + header);
    if(received_bytes == 0) {
        delete []packet;
        return false;
    }
    if(received_bytes <= header) {
        delete []packet;
        return false;
    }
    uint32_t packet_sequence = 0;
    uint32_t packet_ack = 0;
    uint32_t packet_ack_bits = 0;
    readHeader(packet, packet_sequence, packet_ack, packet_ack_bits);
    m_ReliabilitySystem.packetReceived(packet_sequence, received_bytes - header);
    m_ReliabilitySystem.processAck(packet_ack, packet_ack_bits);
    memcpy(data, packet + header, received_bytes - header);

    delete []packet;
    return received_bytes - header;
}

void ReliableConnection::update(float dt) {
    Connection::update(dt);
    m_ReliabilitySystem.update(dt);
}

int ReliableConnection::headerSize() const {
    return Connection::headerSize() + m_ReliabilitySystem.headerSize();
}

ReliabilitySystem &ReliableConnection::reliabilitySystem() {
    return m_ReliabilitySystem;
}

void ReliableConnection::writeInteger(uint8_t * data, uint32_t value) {
    data[0] = (uint8_t) (value >> 24);
    data[1] = (uint8_t) ((value >> 16) & 0xFF);
    data[2] = (uint8_t) ((value >> 8) & 0xFF);
    data[3] = (uint8_t) (value & 0xFF);
}

void ReliableConnection::writeHeader(uint8_t * header, uint32_t sequence, uint32_t ack, uint32_t ack_bits) {
    writeInteger(header, sequence);
    writeInteger(header + 4, ack);
    writeInteger(header + 8, ack_bits);
}

void ReliableConnection::readInteger(const uint8_t * data, uint32_t & value) {
     value = (((uint32_t)data[0] << 24) | ((uint32_t)data[1] << 16) |
              ((uint32_t)data[2] << 8)  | ((uint32_t)data[3]));
}

void ReliableConnection::readHeader(const uint8_t * header, uint32_t & sequence, uint32_t & ack, uint32_t & ack_bits) {
    readInteger(header, sequence);
    readInteger(header + 4, ack);
    readInteger(header + 8, ack_bits);
}

void ReliableConnection::onStop() {
    clearData();
}

void ReliableConnection::onDisconnect() {
    clearData();
}

void ReliableConnection::clearData() {
    m_ReliabilitySystem.reset();
}
