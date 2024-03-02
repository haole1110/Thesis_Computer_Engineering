#include <stdint.h>
/*
 * sensor_data.h
 *
 *  Created on: Oct 23, 2023
 *      Author: Huy Ly
 */

#ifndef INC_SENSOR_DATA_H_
#define INC_SENSOR_DATA_H_

#include <ArduinoJson.h>

struct AllSensorData
{
  /* data */
  float temp = 9999, humi = 9999, CO = 9999, CO2 = 9999, SO2 = 9999, NO2 = 9999, PM25 = 9999, PM10 = 9999, O3 = 9999;
};


class SENSOR_DATA{
  public:
    SENSOR_DATA(){};
    String floatToString(float value);
};

class SENSOR_RS485{
  private:
    uint8_t* data_temperature;
    uint8_t* data_humidity;
    uint8_t* data_co;
    uint8_t* data_co2;
    uint8_t* data_so2;
    uint8_t* data_no2;
    uint8_t* data_pm2_5;
    uint8_t* data_o3;
    uint8_t* data_pm10;

  public:
    SENSOR_RS485();
    ~SENSOR_RS485();
    uint8_t* getTemperature();
    uint8_t* getHumidity();
    uint8_t* getCO();
    uint8_t* getCO2();
    uint8_t* getSO2();
    uint8_t* getNO2();
    uint8_t* getPM2_5();
    uint8_t* getO3();
    uint8_t* getPM10();
};

AllSensorData GetAllSensorData();
const char* ConvertToJsonObject(AllSensorData InData);
// AllSensorData roundData(AllSensorData data);

#endif