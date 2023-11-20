#include "network.h"

#include "networksystem.h"

#include <cstring>

#ifdef SHARED_DEFINE
Module *moduleCreate(Engine *engine) {
    return new Network(engine);
}
#endif

static const char *meta = \
"{"
"   \"version\": \"1.0\","
"   \"module\": \"Network\","
"   \"description\": \"Network Module\","
"   \"author\": \"Evgeniy Prikazchikov\","
"   \"objects\": {"
"       \"NetworkSystem\": \"system\","
"   },"
"   \"components\": ["
"       \"NetworkServer\","
"       \"NetworkClient\""
"   ]"
"}";

Network::Network(Engine *engine) :
        Module(engine),
        m_pSystem(new NetworkSystem()) {
}

Network::~Network() {
    delete m_pSystem;
}

const char *Network::metaInfo() const {
    return meta;
}

void *Network::getObject(const char *name) {
    if(strcmp(name, "NetworkSystem") == 0) {
        return m_pSystem;
    }

    return nullptr;
}
