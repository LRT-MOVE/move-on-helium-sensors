#pragma once

#include <mbed.h>
#include <include/protocol.h>

class Storage {
public:
    Storage();

    void lock();
    void unlock();
    void update();

    CDHPacket packet;
    SensorData* const data;
private:
    Mutex data_mutex;
};
