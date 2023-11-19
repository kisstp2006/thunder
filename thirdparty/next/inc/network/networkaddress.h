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

#ifndef NETWORKADDRESS_H
#define NETWORKADDRESS_H

#include <cstdint>

#include <variant.h>

class NEXT_LIBRARY_EXPORT NetworkAddress {
public:
    NetworkAddress();

    NetworkAddress(uint32_t address, uint16_t port);

    NetworkAddress(const string &address, uint16_t port);

    uint32_t toIPv4Adress() const;

    uint16_t port() const;

    bool operator==(const NetworkAddress &right) const;

    bool operator!=(const NetworkAddress &right) const;

    bool operator<(const NetworkAddress &right) const;

private:
    uint32_t m_address;

    uint16_t m_port;

};

#endif // NETWORKADDRESS_H
