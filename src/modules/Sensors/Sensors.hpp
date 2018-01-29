#pragma once

#include <mbed.h>
#include <modules/ADC/MPC320X.hpp>
#include <modules/ENV/BME680.hpp>
#include <modules/IMU/BNO055IMU.hpp>
#include <modules/CDH/CDHUart.hpp>
#include <modules/Temperature/DS18B20.hpp>
#include <lib/MODSERIAL/MODSERIAL.h>
#include <lib/DS1820/DS1820.h>

class Sensors {
public:
    Sensors() :
        // Interfaces
        led(LED1),
        logger(A7, USBRX),
        cdh(D1, D0),
        gps(D4, D5),
        bno(D12, A6),
        spi(D2, A5, A1),
        cs_bme680(D3, 1),
        cs_adc(D6, 1),

        // CDH data
        cdhuart(cdh, &storage),

        // Sensors
        imu(bno, &storage),
        env(spi, cs_bme680, &storage),
        adc(spi, cs_adc),
        temperature(D9, &storage)
        {};

    void setup();
    void loop();
    void log();

private:
    //Interfaces
    DigitalOut led;
    Serial logger;
    MODSERIAL cdh;
    I2C gps;
    I2C bno;
    SPI spi;
    DigitalOut cs_bme680;
    DigitalOut cs_adc;

    // CDH data
    Storage storage;
    CDHUart cdhuart;

    // CDH data
    Storage storage;
    CDHUart cdhuart;

    // Sensors
    BNO055IMU imu;
    BME680 env;
    MPC320X adc;
    DS18B20 temperature;

};
