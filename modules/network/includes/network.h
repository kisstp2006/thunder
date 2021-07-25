#ifndef NETWORK_H
#define NETWORK_H

#include <module.h>

class Network : public Module {
public:
    Network(Engine *engine);

    ~Network();

    const char *description() const;

    const char *version() const;

    uint8_t types() const;

    System *system();

protected:
    Engine *m_pEngine;

    System *m_pSystem;
};
#ifdef NEXT_SHARED
extern "C" {
    MODULE_EXPORT Module *moduleCreate(Engine *engine);
}
#endif
#endif // NETWORK_H
