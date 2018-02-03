#include "Sensors.hpp"

void Sensors::setup() {
    printf("\r\nRESET\r\n");
    imu.start();
    env.start();
    temperature.start();
}


void Sensors::loop() {

    while(1) {
        log();
        Thread::wait(1000);
    }
}

void Sensors::log() {
    storage.lock();
    SensorData* data = storage.data;
    logger.printf("ENV "
                  "T=%.4f,H=%.4f,P=%.4f,G=%.4f"
                  "\r\n",
                  data->env.temperature,
                  data->env.humidity,
                  data->env.pressure,
                  data->env.gasresistance);
    logger.printf("IMU "
                  "ACC_X=%.4f,ACC_Y=%.4f,ACC_Z=%.4f,"
                  "MAG_X=%.4f,MAG_Y=%.4f,MAG_Z=%.4f,"
                  "GYRO_X=%.4f,GYRO_Y=%.4f,GYRO_Z=%.4f,"
                  "QUAT_W=%.4f,QUAT_X=%.4f,QUAT_Y=%.4f,QUAT_Z=%.4f,"
                  "ANG_X=%.4f,ANG_Y=%.4f,ANG_Z=%.4f,"
                  "TEMP_ACC=%d,TEMP_GYRO=%d,"
                  "RST_TMP=%u,RST_ZRO=%u"
                  "\r\n",
                  data->imu.accel.x, data->imu.accel.y, data->imu.accel.z,
                  data->imu.mag.x, data->imu.mag.y, data->imu.mag.z,
                  data->imu.gyro.x, data->imu.gyro.y, data->imu.gyro.z,
                  data->imu.quaternion.w, data->imu.quaternion.x, data->imu.quaternion.y, data->imu.quaternion.z,
                  data->imu.orientation.x, data->imu.orientation.y, data->imu.orientation.z,
                  data->imu.temp_accel, data->imu.temp_gyro,
                  data->imu.resets_temps, data->imu.resets_zeroes);
    if(temperature.getNumDevices() > 0) {
        logger.printf("TMP ");
        logger.printf("T%d=%.4f", 0, data->temp[0].temp);
        for(int i = 1; i < temperature.getNumDevices(); i++) {
            logger.printf(",T%d=%.4f", i, data->temp[i].temp);
        }
        logger.printf("\r\n");
    }
    storage.unlock();
}
