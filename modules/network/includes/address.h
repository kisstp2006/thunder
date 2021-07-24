#ifndef ADDRESS_H
#define ADDRESS_H

#include <cstdint>

class Address {
public:
    Address();

    Address(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port);

    Address(uint32_t address, uint16_t port);

    uint32_t address() const;

    uint8_t a() const;

    uint8_t b() const;

    uint8_t c() const;

    uint8_t d() const;

    uint16_t port() const;

    bool operator==(const Address &right) const;

    bool operator!=(const Address &right) const;

    bool operator<(const Address &right) const;

private:
    uint32_t    m_Address;
    uint16_t    m_Port;

};

#endif // ADDRESS_H
