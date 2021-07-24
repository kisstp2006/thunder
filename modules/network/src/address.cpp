#include "address.h"

Address::Address() {
    m_Address = 0;
    m_Port = 0;
}

Address::Address(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port) {
    m_Address = (a << 24) | (b << 16) | (c << 8) | d;
    m_Port = port;
}

Address::Address(uint32_t address, uint16_t port) {
    m_Address = address;
    m_Port = port;
}

uint32_t Address::address() const {
    return m_Address;
}

uint8_t Address::a() const {
    return static_cast<uint8_t>(m_Address >> 24);
}

uint8_t Address::b() const {
    return static_cast<uint8_t>(m_Address >> 16);
}

uint8_t Address::c() const {
    return static_cast<uint8_t>(m_Address >> 8);
}

uint8_t Address::d() const {
    return static_cast<uint8_t>(m_Address);
}

uint16_t Address::port() const {
    return m_Port;
}

bool Address::operator==(const Address &right) const {
    return m_Address == right.m_Address && m_Port == right.m_Port;
}

bool Address::operator!=(const Address &right) const {
    return !(*this == right);
}

bool Address::operator<(const Address &right) const {
    // note: this is so we can use address as a key in std::map
    if(m_Address < right.m_Address) {
        return true;
    }
    if(m_Address > right.m_Address) {
        return false;
    } else {
        return m_Port < right.m_Port;
    }
}
