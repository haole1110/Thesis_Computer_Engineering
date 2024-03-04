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
  float temp = 1, humi = 1, CO = 1, CO2 = 1, SO2 = 1, NO2 = 1, PM25 = 1, PM10 = 1, O3 = 1;
};


class SENSOR_DATA{
  public:
    SENSOR_DATA(){};
    String floatToString(float value);
};

class SENSOR_RS485{
  private:
    uint8_t* cmd_temp_humi;
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
    uint8_t* getTempAndHumi();
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