#pragma once

#include <mbed.h>
#include <algorithm>
#include <include/pinmap.h>
#include <lib/MODSERIAL/MODSERIAL.h>
#include <lib/Storage/Storage.hpp>
#include <lib/SyncSerial/SyncSerial.hpp>
#include <lib/mbed-memory-status/mbed_memory_status.h>
#include <modules/ADCS/Sidepanel.hpp>
#include <modules/ENV/BME680.hpp>
#include <modules/IMU/BNO055IMU.hpp>
#include <modules/CDH/CDHUart.hpp>
#include <modules/TOSS/Toss.hpp>
#include <modules/Temperature/DS18B20.hpp>
#include <modules/GPS/GPS.hpp>


class Sensors {
public:
    Sensors() :
        // Interfaces
        logger(LOG_UART, USBRX),
        cdh(CDH_TX, CDH_RX),
        i2c_gps(GPS_SDA, GPS_SCL),
        i2c_imu(IMU_SDA, IMU_SCL),
        spi(SPI_MOSI, SPI_MISO, SPI_SCK),
        cs_adcs(SPI_CS_ADCS, 1),
        cs_env0(SPI_CS_ENV0, 1),
        cs_env1(SPI_CS_ENV1, 1),
        cs_sun(SPI_CS_SUN, 1),

        // CDH data
        cdhuart(cdh, &storage),

        // Sensors
        imu(i2c_imu, IMU_RST, &storage, &logger),
        env0(&spi, &cs_env0, 0, &storage, &logger),
        env1(&spi, &cs_env1, 1, &storage, &logger),
        adcs(&spi, &cs_adcs, &storage, &logger),
        temperature(TEMP_OW, &storage, &logger),
        toss(&spi, &cs_sun, &storage, &logger),
        gps(&i2c_gps, &storage, &logger)
        {};

    void setup();
    void loop();
    void log(uint16_t log_ms);
    uint8_t check_sensor_status();

private:
    //Interfaces
    SyncSerial logger;
    MODSERIAL cdh;
    I2C i2c_gps;
    I2C i2c_imu;
    SPI spi;
    DigitalOut cs_adcs;
    DigitalOut cs_env0;
    DigitalOut cs_env1;
    DigitalOut cs_sun;

    // CDH data
    Storage storage;
    CDHUart cdhuart;

    // Sensors
    BNO055IMU imu;
    BME680 env0;
    BME680 env1;
    Sidepanel adcs;
    DS18B20 temperature;
    Toss toss;
    GPS gps;
};
