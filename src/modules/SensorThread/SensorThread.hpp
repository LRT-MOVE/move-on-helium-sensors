#pragma once

#include <mbed.h>

class SensorThread {
public:
    SensorThread() = default;

    /**
     * Start thread
     */
    void start();

    /**
     * Thread loop func
     */
    void loop();

    /**
     * Inital sensor config
     */
    virtual bool setup() = 0;

    /**
     * Read sensor data and update datastructure
     */
    virtual void update() = 0;

    /**
     * Set update rate in ms for the thread loop.
     * A value of 0 causes a continously expecting a wait as part of the update
     * function.
     */
    void set_update_rate(uint16_t update_rate);
    void set_priority(osPriority priority);

private:
    Thread _thread;

    uint16_t update_rate = 1000;
};