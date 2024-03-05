#include "sensor_data.h"

/*..........................Implement SENSOR_DATA class.........................*/

SENSOR_DATA::SENSOR_DATA(){
  temp = 1;
  humi = 1;
  CO = 1;
  CO2 = 1;
  SO2 = 1;
  NO2 = 1;
  PM25 = 1; 
  PM10 = 1;
  O3 = 1;
}

void SENSOR_DATA::updateTemp(){
  // GET TEMP DATA
  Serial.println("Update to temp with data...");
  Serial2.write(data.getRequestTemp(), 8);
  delay(1000);
  if (Serial2.available()) {
    uint8_t receivedData[7];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    temp = (int)receivedData[3] * 256 + (int)receivedData[4];
    temp /= 10;
  }
};

void SENSOR_DATA::updateHumi(){
  // GET HUMI DATA
  Serial.println("Update to humi with data...");
  Serial2.write(data.getRequestHumi(), 8);
  delay(1000);
  if (Serial2.available()) {
    uint8_t receivedData[7];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    humi = (int)receivedData[3] * 256 + (int)receivedData[4];
    humi /= 10;
  }
};

void SENSOR_DATA::updateCO(){
  // GET CO DATA
  Serial.println("Update to CO with data...");
  Serial2.write(data.getRequestCO(), 8);
  delay(1000);
  if (Serial2.available()) {
    uint8_t receivedData[7];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    CO = (int)receivedData[3] * 256 + (int)receivedData[4];
  }
};

void SENSOR_DATA::updateCO2(){
  // GET CO2 DATA
  Serial.println("Update to CO2 with data...");
  Serial2.write(data.getRequestCO2(), 8);
  delay(1000);
  if (Serial2.available()) {
    uint8_t receivedData[7];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    CO2 = (int)receivedData[3] * 256 + (int)receivedData[4];
  }
};

void SENSOR_DATA::updateSO2(){
  // GET SO2 DATA
  Serial.println("Update to SO2 with data...");
  Serial2.write(data.getRequestSO2(), 8);
  delay(1000);
  if (Serial2.available()) {
    uint8_t receivedData[7];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    SO2 = (int)receivedData[3] * 256 + (int)receivedData[4];
  }
};

void SENSOR_DATA::updateNO2(){
  // GET NO2 DATA
  Serial.println("Update to NO2 with data...");
  Serial2.write(data.getRequestNO2(), 8);
  delay(1000);
  if (Serial2.available()) {
    uint8_t receivedData[7];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    NO2 = (int)receivedData[3] * 256 + (int)receivedData[4];
  }
};

void SENSOR_DATA::updatePM25(){
  // GET PM2.5 DATA
  Serial.println("Update to PM2.5 with data...");
  Serial2.write(data.getRequestPM25(), 8);
  delay(1000);
  if (Serial2.available()) {
    uint8_t receivedData[7];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    PM25 = (int)receivedData[3] * 256 + (int)receivedData[4];
  }
};

void SENSOR_DATA::updatePM10(){
  // GET PM10 DATA
  Serial.println("Update to PM10 with data...");
  Serial2.write(data.getRequestPM10(), 8);
  delay(1000);
  if (Serial2.available()) {
    uint8_t receivedData[7];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    PM10 = (int)receivedData[3] * 256 + (int)receivedData[4];
  }
};

void SENSOR_DATA::updateO3(){
  // GET O3 DATA
  Serial.println("Update to O3 with data...");
  Serial2.write(data.getRequestO3(), 8);
  delay(1000);
  if (Serial2.available()) {
    uint8_t receivedData[7];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    O3 = (int)receivedData[3] * 256 + (int)receivedData[4];
  }
};

AllSensorData SENSOR_DATA::getAllData(){
  updateTemp();
  updateHumi();
  updateCO();
  updateCO2();
  updateSO2();
  updateNO2();
  updatePM25();
  updatePM10();
  updateO3();

  AllSensorData tempData;
  tempData.temp = temp;
  tempData.humi = humi;
  tempData.CO = CO;
  tempData.CO2 = CO2;
  tempData.SO2 = SO2;
  tempData.NO2 = NO2;
  tempData.PM25 = PM25;
  tempData.PM10 = PM10;
  tempData.O3 = O3;

  return tempData;
};

String SENSOR_DATA::floatToString(float value) {
  char buffer[20];  // Đủ lớn để chứa chuỗi
  sprintf(buffer, "%.2f", value);
  return String(buffer);
}

/*.........................Implement SENSOR_RS485 class............................*/

SENSOR_RS485::SENSOR_RS485(){
  request_temp = new uint8_t[8]{3, 3, 0, 0, 0, 1, 133, 232};
  request_humi = new uint8_t[8]{3, 3, 0, 1, 0, 1, 212, 40};
  request_CO = new uint8_t[8]{0x0E, 0x03, 0x00, 0x02, 0x00, 0x01, 0x25, 0x35};
  request_CO2 = new uint8_t[8]{2, 3, 0, 4, 0, 1, 197, 248};
  request_SO2 = new uint8_t[8]{0x0D, 0x03, 0x00, 0x02, 0x00, 0x01, 0x25, 0x06};
  request_NO2 = new uint8_t[8]{0x0C, 0x03, 0x00, 0x02, 0x00, 0x01, 0x24, 0xD7};
  request_PM25 = new uint8_t[8]{4, 3, 0, 12, 0, 1, 68, 92};
  request_O3 = new uint8_t[8]{11, 3, 0, 2, 0, 1, 37, 96};
  request_PM10 = new uint8_t[8]{4, 3, 0, 13, 0, 1, 21, 156};
};

SENSOR_RS485::~SENSOR_RS485() {
    delete[] request_temp;
    delete[] request_humi;
    delete[] request_CO;
    delete[] request_CO2;
    delete[] request_SO2;
    delete[] request_NO2;
    delete[] request_PM25;
    delete[] request_O3;
    delete[] request_PM10;
};

// Hàm để lấy con trỏ tới giá trị nhiệt độ
uint8_t* SENSOR_RS485::getRequestTemp() {
    return request_temp;
}

// Hàm để lấy con trỏ tới chuỗi request độ ẩm
uint8_t* SENSOR_RS485::getRequestHumi() {
    return request_humi;
}

// Hàm để lấy con trỏ tới chuỗi request CO
uint8_t* SENSOR_RS485::getRequestCO() {
    return request_CO;
}

// Hàm để lấy con trỏ tới chuỗi request CO2
uint8_t* SENSOR_RS485::getRequestCO2() {
    return request_CO2;
}

// Hàm để lấy con trỏ tới chuỗi request SO2
uint8_t* SENSOR_RS485::getRequestSO2() {
    return request_SO2;
}

// Hàm để lấy con trỏ tới chuỗi request NO2
uint8_t* SENSOR_RS485::getRequestNO2() {
    return request_NO2;
}

// Hàm để lấy con trỏ tới chuỗi request PM25
uint8_t* SENSOR_RS485::getRequestPM25() {
    return request_PM25;
}

// Hàm để lấy con trỏ tới chuỗi request PM10
uint8_t* SENSOR_RS485::getRequestPM10() {
    return request_PM10;
}

// Hàm để lấy con trỏ tới chuỗi request O3
uint8_t* SENSOR_RS485::getRequestO3() {
    return request_O3;
}


const char* ConvertToJsonObject(AllSensorData data, Relay relay) { 
  // Khởi tạo đối tượng JSON
  DynamicJsonDocument doc(1024);

  // Thêm các trường vào đối tượng JSON
  doc["station_id"] = "air_0002";
  doc["station_name"] = "NBIOT 0002";

  // Tạo mảng các cảm biến
  JsonArray sensors = doc.createNestedArray("sensors");

  // Thêm các đối tượng cảm biến vào mảng
  JsonObject sensor1 = sensors.createNestedObject();
  sensor1["id"] = "temp_0001";
  sensor1["value"] = round(data.temp * 100) / 100.0;

  JsonObject sensor2 = sensors.createNestedObject();
  sensor2["id"] = "humi_0001";
  sensor2["value"] = round(data.humi * 100) / 100.0;

  JsonObject sensor3 = sensors.createNestedObject();
  sensor3["id"] = "CO_0002";
  sensor3["value"] = round(data.CO * 100) / 100.0;

  JsonObject sensor4 = sensors.createNestedObject();
  sensor4["id"] = "CO2_0002";
  sensor4["value"] = round(data.CO2 * 100) / 100.0;

  JsonObject sensor5 = sensors.createNestedObject();
  sensor5["id"] = "SO2_0002";
  sensor5["value"] = round(data.SO2 * 100) / 100.0;

  JsonObject sensor6 = sensors.createNestedObject();
  sensor6["id"] = "NO2_0002";
  sensor6["value"] = round(data.NO2 * 100) / 100.0;

  JsonObject sensor7 = sensors.createNestedObject();
  sensor7["id"] = "PM25_0002";
  sensor7["value"] = round(data.PM25 * 100) / 100.0;

  JsonObject sensor8 = sensors.createNestedObject();
  sensor8["id"] = "PM10_0002";
  sensor8["value"] = round(data.PM10 * 100) / 100.0;

  JsonObject sensor9 = sensors.createNestedObject();
  sensor9["id"] = "O3_0002";
  sensor9["value"] = round(data.O3 * 100) / 100.0;

  JsonObject sensor10 = sensors.createNestedObject();
  sensor10["id"] = "Relay_0001";
  sensor10["value"] = relay.rl1;

  JsonObject sensor11 = sensors.createNestedObject();
  sensor11["id"] = "Relay_0002";
  sensor11["value"] = relay.rl2;

  JsonObject sensor12 = sensors.createNestedObject();
  sensor12["id"] = "Relay_0003";
  sensor12["value"] = relay.rl3;

  JsonObject sensor13 = sensors.createNestedObject();
  sensor13["id"] = "Relay_0004";
  sensor13["value"] = relay.rl4;


  String temp;
  serializeJson(doc, temp);
  doc.clear();
  const char* charPtr = temp.c_str();
  return charPtr;

}