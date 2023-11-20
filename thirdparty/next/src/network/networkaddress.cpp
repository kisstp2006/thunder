/*
    This file is part of Thunder Next.

    Thunder Next is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    Thunder Next is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Thunder Next.  If not, see <http://www.gnu.org/licenses/>.

    Copyright: 2008-2023 Evgeniy Prikazchikov
*/

#include "networkaddress.h"

#include "socket.h"

#include <cstring>

NetworkAddress::NetworkAddress() :
        m_address(0),
        m_port(0) {

}

NetworkAddress::NetworkAddress(uint32_t address, uint16_t port) :
        m_address(htonl(address)),
        m_port(port) {

}

NetworkAddress::NetworkAddress(const string &address, uint16_t port) :
        m_address(0),
        m_port(port) {

    struct hostent *he = ::gethostbyname(address.c_str());
    if(he) {
        memcpy(&m_address, he->h_addr, he->h_length);
    } else {
        m_address = ::inet_addr(address.c_str());
    }
}

uint32_t NetworkAddress::toIPv4Adress() const {
    return m_address;
}

uint16_t NetworkAddress::port() const {
    return m_port;
}

bool NetworkAddress::operator==(const NetworkAddress &right) const {
    return m_address == right.m_address && m_port == right.m_port;
}

bool NetworkAddress::operator!=(const NetworkAddress &right) const {
    return !(*this == right);
}

bool NetworkAddress::operator<(const NetworkAddress &right) const {
    return (m_address < right.m_address);
}
