/*
 * sensor_data.h
 *
 *  Created on: Oct 23, 2023
 *      Author: Huy Ly
 */

#include "sensor_data.h"

String SENSOR_DATA::floatToString(float value) {
  char buffer[20];  // Đủ lớn để chứa chuỗi
  sprintf(buffer, "%.2f", value);
  return String(buffer);
}

///////////////////////////////////////

SENSOR_RS485::SENSOR_RS485(){
    data_temperature = new uint8_t[8]{3, 3, 0, 0, 0, 1, 133, 232};
    data_humidity = new uint8_t[8]{3, 3, 0, 1, 0, 1, 212, 40};
    data_co = new uint8_t[8]{0x0E, 0x03, 0x00, 0x02, 0x00, 0x01, 0x25, 0x35};
    data_co2 = new uint8_t[8]{2, 3, 0, 4, 0, 1, 197, 248};
    data_so2 = new uint8_t[8]{0x0D, 0x03, 0x00, 0x02, 0x00, 0x01, 0x25, 0x06};
    data_no2 = new uint8_t[8]{0x0C, 0x03, 0x00, 0x02, 0x00, 0x01, 0x24, 0xD7};
    data_pm2_5 = new uint8_t[8]{4, 3, 0, 12, 0, 1, 68, 92};
    data_o3 = new uint8_t[8]{11, 3, 0, 2, 0, 1, 37, 96};
    data_pm10 = new uint8_t[8]{4, 3, 0, 13, 0, 1, 21, 156};
};

SENSOR_RS485::~SENSOR_RS485() {
    delete[] data_temperature;
    delete[] data_humidity;
    delete[] data_co;
    delete[] data_co2;
    delete[] data_so2;
    delete[] data_no2;
    delete[] data_pm2_5;
    delete[] data_o3;
    delete[] data_pm10;
};

// Hàm để lấy con trỏ tới giá trị nhiệt độ
uint8_t* SENSOR_RS485::getTemperature() {
    return data_temperature;
}

// Hàm để lấy con trỏ tới giá trị độ ẩm
uint8_t* SENSOR_RS485::getHumidity() {
    return data_humidity;
}

// Hàm để lấy con trỏ tới giá trị CO
uint8_t* SENSOR_RS485::getCO() {
    return data_co;
}

// Hàm để lấy con trỏ tới giá trị CO2
uint8_t* SENSOR_RS485::getCO2() {
    return data_co2;
}

// Hàm để lấy con trỏ tới giá trị SO2
uint8_t* SENSOR_RS485::getSO2() {
    return data_so2;
}

// Hàm để lấy con trỏ tới giá trị NO2
uint8_t* SENSOR_RS485::getNO2() {
    return data_no2;
}

// Hàm để lấy con trỏ tới giá trị PM2.5
uint8_t* SENSOR_RS485::getPM2_5() {
    return data_pm2_5;
}

// Hàm để lấy con trỏ tới giá trị O3
uint8_t* SENSOR_RS485::getO3() {
    return data_o3;
}

// Hàm để lấy con trỏ tới giá trị PM10
uint8_t* SENSOR_RS485::getPM10() {
    return data_pm10;
}

AllSensorData GetAllSensorData() {
    SENSOR_RS485 data485;
    AllSensorData ReturnData;

    // GET TEMP DATA
    Serial.println("Writing to temp with data...");
    Serial2.write(data485.getTemperature(), 8);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    delay(1000);
    if (Serial2.available()) {
    uint8_t receivedData[7];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    ReturnData.temp = (int)receivedData[3] * 256 + (int)receivedData[4];
    ReturnData.temp /= 10;
    Serial.println(ReturnData.temp);
    }

    // GET HUMI DATA
    Serial.println("Writing to humi with data...");
    Serial2.write(data485.getHumidity(), 8);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    delay(1000);
    if (Serial2.available()) {
    uint8_t receivedData[7];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    ReturnData.humi = (int)receivedData[3] * 256 + (int)receivedData[4];
    ReturnData.humi /= 10;
    Serial.println(ReturnData.humi);
    }

    // GET CO DATA
    Serial.println("Writing to CO with data...");
    Serial2.write(data485.getCO(), 8);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    delay(1000);
    if (Serial2.available()) {
    uint8_t receivedData[7];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    ReturnData.CO = (int)receivedData[3] * 256 + (int)receivedData[4];
    // CO /= 10;
    Serial.println(ReturnData.CO);
    }

    // GET CO2 DATA
    Serial.println("Writing to CO2 with data...");
    Serial2.write(data485.getCO2(), 8);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    delay(1000);
    if (Serial2.available()) {
    uint8_t receivedData[7];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    ReturnData.CO2 = (int)receivedData[3] * 256 + (int)receivedData[4];
    // CO2 /= 10;
    Serial.println(ReturnData.CO2);
    }

    // GET SO2 DATA
    Serial.println("Writing to SO2 with data...");
    Serial2.write(data485.getSO2(), 8);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    delay(1000);
    if (Serial2.available()) {
    uint8_t receivedData[7];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    ReturnData.SO2 = (int)receivedData[3] * 256 + (int)receivedData[4];
    // SO2 /= 10;
    Serial.println(ReturnData.SO2);
    }

    // GET NO2 DATA
    Serial.println("Writing to NO2 with data...");
    Serial2.write(data485.getNO2(), 8);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    delay(1000);
    if (Serial2.available()) {
    uint8_t receivedData[7];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    ReturnData.NO2 = (int)receivedData[3] * 256 + (int)receivedData[4];
    // NO2 /= 10;
    Serial.println(ReturnData.NO2);
    }

    // GET PM2.5 DATA
    Serial.println("Writing to PM2.5 with data...");
    Serial2.write(data485.getPM2_5(), 8);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    delay(1000);
    if (Serial2.available()) {
    uint8_t receivedData[7];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    ReturnData.PM25 = (int)receivedData[3] * 256 + (int)receivedData[4];
    // PM25 /= 10;
    Serial.println(ReturnData.PM25);
    }

    // GET PM10 DATA
    Serial.println("Writing to PM10 with data...");
    Serial2.write(data485.getPM10(), 8);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    delay(1000);
    if (Serial2.available()) {
    uint8_t receivedData[7];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    ReturnData.PM10 = (int)receivedData[3] * 256 + (int)receivedData[4];
    // PM10 /= 10;
    Serial.println(ReturnData.PM10);
    }

    // GET O3 DATA
    Serial.println("Writing to O3 with data...");
    Serial2.write(data485.getO3(), 8);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    delay(1000);
    if (Serial2.available()) {
    uint8_t receivedData[7];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    ReturnData.O3 = (int)receivedData[3] * 256 + (int)receivedData[4];
    // O3 /= 10;
    Serial.println(ReturnData.O3);
    }
    return  ReturnData;
}

const char* ConvertToJsonObject(AllSensorData data) { 
    DynamicJsonDocument doc(1024);

    // doc["sensor"] = "gps";
    // doc["time"] = 1351824120;
    // doc["data"][0] = 48.756080;
    // doc["data"][1] = 2.302038;

    doc["temperature"] = round(data.temp * 100) / 100.0;
    doc["humidity"] = round(data.humi * 100) / 100.0;
    doc["co2"] = round(data.CO2 * 100) / 100.0;
    doc["co"] = round(data.CO * 100) / 100.0;
    doc["so2"] = round(data.SO2 * 100) / 100.0;
    doc["no2"] = round(data.NO2 * 100) / 100.0;
    doc["pm2-5"] = round(data.PM25 * 100) / 100.0;
    doc["pm10"] = round(data.PM10 * 100) / 100.0;
    doc["o3"] = round(data.O3 * 100) / 100.0;

    String temp;
    serializeJson(doc, temp);
    doc.clear();
    const char* charPtr = temp.c_str();
    return charPtr;
    // char* temp;
    // return temp;
}

// AllSensorData roundData(AllSensorData data){
//   data.temp = round(data.temp * 100) / 100.0;
//   data.humi = round(data.humi * 100) / 100.0;
//   data.CO = round(data.CO * 100) / 100.0;
//   data.CO2 = round(data.CO2 * 100) / 100.0;
//   data.SO2 = round(data.SO2 * 100) / 100.0;
//   data.O3 = round(data.O3 * 100) / 100.0;
//   data.PM10 = round(data.PM10 * 100) / 100.0;
//   data.PM25 = round(data.PM25 * 100) / 100.0;
//   data.NO2 = round(data.NO2 * 100) / 100.0;
//   return data;
// }