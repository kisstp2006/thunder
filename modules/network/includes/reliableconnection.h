#ifndef RELIABLECONNECTION_H
#define RELIABLECONNECTION_H

#include <cstdint>

#include "connection.h"
#include "reliabilitysystem.h"

class ReliableConnection : public Connection {
public:

    ReliableConnection(uint32_t protocolId, float timeout, uint32_t maxSequence = 0xFFFFFFFF);

    ~ReliableConnection();

    bool sendPacket(const uint8_t data[], int size) override;

    int receivePacket(uint8_t data[], int size) override;

    void update(float dt) override;

    int headerSize() const;

    ReliabilitySystem &reliabilitySystem();

protected:

    void writeInteger(uint8_t * data, uint32_t value);

    void writeHeader(uint8_t * header, uint32_t sequence, uint32_t ack, uint32_t ack_bits );

    void readInteger(const uint8_t * data, uint32_t & value );

    void readHeader(const uint8_t * header, uint32_t & sequence, uint32_t & ack, uint32_t & ack_bits );

    virtual void onStop();

    virtual void onDisconnect();

private:
    void clearData();

    ReliabilitySystem m_ReliabilitySystem;    // reliability system: manages sequence numbers and acks, tracks network stats etc.
};

#endif // RELIABLECONNECTION_H
