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

#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "socket.h"

class NEXT_LIBRARY_EXPORT UdpSocket : public Socket {
public:
    bool bind(const NetworkAddress &address) override;

    uint64_t readDatagram(int8_t *data, uint64_t maxSize, NetworkAddress *address);

    uint64_t writeDatagram(int8_t *data, uint64_t size, const NetworkAddress &address);

};

#endif // UDPSOCKET_H
