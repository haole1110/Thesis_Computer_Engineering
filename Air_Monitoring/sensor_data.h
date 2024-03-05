#include <stdint.h>

#ifndef INC_SENSOR_DATA_H_
#define INC_SENSOR_DATA_H_

#include <ArduinoJson.h>
#include "relay.h"

struct AllSensorData
{
  /* data */
  float temp = 1, humi = 1, CO = 1, CO2 = 1, SO2 = 1, NO2 = 1, PM25 = 1, PM10 = 1, O3 = 1;
};



class SENSOR_DATA{
  private:
    float temp;
    float humi;
    float CO;
    float CO2;
    float SO2;
    float NO2;
    float PM25; 
    float PM10;
    float O3;
    SENSOR_RS485 data485;
  public:
    SENSOR_DATA(){};
    void updateTemp();
    void updateHumi();
    void updateCO();
    void updateCO2();
    void updateSO2();
    void updateNO2();
    void updatePM25();
    void updatePM10();
    void updateO3();
    AllSensorData getAllData();
    String floatToString(float value);
};

class SENSOR_RS485{
  private:
    uint8_t* request_temp;
    uint8_t* request_humi;
    uint8_t* request_CO;
    uint8_t* request_CO2;
    uint8_t* request_SO2;
    uint8_t* request_NO2;
    uint8_t* request_PM25;
    uint8_t* request_PM10;
    uint8_t* request_O3;

  public:
    SENSOR_RS485();
    ~SENSOR_RS485();
    uint8_t* getRequestTemp();
    uint8_t* getRequestHumi();
    uint8_t* getRequestCO();
    uint8_t* getRequestCO2();
    uint8_t* getRequestSO2();
    uint8_t* getRequestNO2();
    uint8_t* getRequestPM25();
    uint8_t* getRequestPM10();
    uint8_t* getRequestO3();
};

const char* ConvertToJsonObject(AllSensorData InData, Relay Allrelay);

#endif