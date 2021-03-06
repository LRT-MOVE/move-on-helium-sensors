/*
 * DS18B20.cpp
 *
 *  Created on: Jan 15, 2018
 *      Author: tkale
 */

#include "DS18B20.hpp"

bool DS18B20::setup() {
    // Update once every 2 seconds. Since the sensors need ~700 ms to convert
    // one temperature.
    set_update_rate(2000);

    int retries = MAX_TEMP_SENSORS * 5;
    while(numDevices < MAX_TEMP_SENSORS) {
        if(DS1820::unassignedProbe(dataPin)) {
            sensors[numDevices] = new DS1820(dataPin);
            if (sensors[numDevices]->isValid()) {
                numDevices++;
            }
        }
        if(retries-- <= 0){
            logger->lock();
            logger->printf("%s Could not find all %d Temp sensors!\r\n", _name, MAX_TEMP_SENSORS);
            logger->unlock();
            break;
        }
    }

    // XXX: For some reason we need a manual reset condition on the bus.
    DigitalOut d(dataPin);
    d = 0;
    Thread::wait(1);
    d = 1;
    sensors[0]->convertTemperature(false, DS1820::all_devices);

    logger->lock();
    logger->printf("%s Found %d DS18B20 sensors with %d remaining retries\r\n", _name, numDevices, retries);
    logger->unlock();

    return numDevices;
}

void DS18B20::update() {
    for (int i = 0; i< numDevices; i++) {
        //XXX: Disable interrupts to ensure onewire communication is not interrupted
        __disable_irq();
        temperature[i] = sensors[i]->temperature();
        __enable_irq();
        if (temperature[i] != -1000) {
            data.temp[i] = (int16_t)(temperature[i]*100);
        }
    }
    last_data = storage->get_ts();

    storage->lock();
    storage->data->temp = data;
    storage->unlock();

    // Start temperature conversion, wait until ready
    sensors[0]->convertTemperature(false, DS1820::all_devices);
}

uint8_t DS18B20::getNumDevices() {
    return numDevices;
}

void DS18B20::print() {
    logger->lock();
    if (getNumDevices() > 0) {
        logger->printf("%s T=%lu,"
                       "OW%d=%hd",
                       _name, last_data,
                       0, data.temp[0]);
        for (int i = 1; i < getNumDevices(); i++) {
            logger->printf(",OW%d=%hd", i, data.temp[i]);
        }
        logger->printf("\r\n");
    }
    logger->unlock();
}

DS18B20::~DS18B20() {
    for(uint8_t i = 0; i < numDevices; i++){
        delete(sensors[i]);
    }
}
