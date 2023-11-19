#ifndef NETWORKSYSTEM_H
#define NETWORKSYSTEM_H

#include <system.h>

class Engine;

class NetworkSystem : public System {
public:
    NetworkSystem();
    ~NetworkSystem() override;

    bool init() override { return true; }

    void update(World *world) override;

    int threadPolicy() const override { return Pool; }

};

#endif // NETWORKSYSTEM_H
