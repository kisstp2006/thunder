#include "network.h"

#include <system.h>

#ifdef NEXT_SHARED
Module *moduleCreate(Engine *engine) {
    return new Network(engine);
}
#endif

Network::Network(Engine *engine) :
        m_pEngine(engine),
        m_pSystem(nullptr) {
}

Network::~Network() {
    delete m_pSystem;
}

const char *Network::description() const {
    return "Network Module";
}

const char *Network::version() const {
    return "1.0";
}

uint8_t Network::types() const {
    return SYSTEM;
}

System *Network::system() {
    return m_pSystem;
}
