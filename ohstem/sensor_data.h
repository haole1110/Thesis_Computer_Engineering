#include <stdint.h>

#ifndef INC_SENSOR_DATA_H_
#define INC_SENSOR_DATA_H_

#include <ArduinoJson.h>
#include "relay.h"

struct AllSensorData
{
  /* data */
  float temp = 1, humi = 1, spH = 1, sTemp = 1, sHumi = 1;
  uint16_t sNito = 1, sPhotho = 1, sKali = 1, sConductivity = 1;
};



class SENSOR_DATA{
  private:
    float temp;
    float humi;
    float sTemp;
    float sHumi;
    float spH;
    uint16_t sNito;
    uint16_t sPhotho; 
    uint16_t sKali;
    uint16_t sConductivity;
    SENSOR_RS485 data485;
  public:
    SENSOR_DATA(){};
    void updateTempAndHumi();
    void updateSTempAndSHumi();
    void updateSpH();
    void updateSNPK();
    void updateSConductivity();
    AllSensorData getAllData();
    String floatToString(float value);
};

class SENSOR_RS485{
  private:
    uint8_t* request_temp_humi;
    uint8_t* request_sTemp_sHumi;
    uint8_t* request_spH;
    uint8_t* request_sNPK;
    uint8_t* request_sConductivity;

  public:
    SENSOR_RS485();
    ~SENSOR_RS485();
    uint8_t* getRequestTempHumi();
    uint8_t* getRequestsTempsHumi();
    uint8_t* getRequestspH();
    uint8_t* getRequestsNPK();
    uint8_t* getRequestsConductivity();
};

const char* ConvertToJsonObject(AllSensorData InData, Relay Allrelay);

// AllSensorData roundData(AllSensorData data);

#endif