#include "sensor_data.h"

/*..........................Implement SENSOR_DATA class.........................*/

SENSOR_DATA::SENSOR_DATA(){
  temp = 1;
  humi = 1;
  sTemp = 1;
  sHumi = 1;
  spH = 1;
  sNito = 1;
  sPhotho = 1; 
  sKali = 1;
  sConductivity = 1;
}

void SENSOR_DATA::updateTempAndHumi(){
  // GET TEMP AND HUMI DATA
  Serial.println("Update to temp and humi with data...");
  Serial2.write(data.getRequestTempHumi(), 8);
  delay(1000);
  if (Serial2.available()) {
    uint8_t receivedData[9];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    temp = ((int)receivedData[5]*256 + (int)receivedData[6])/10.0;
    humi = ((int)receivedData[3]*256 + (int)receivedData[4])/10.0;
  }
};

void SENSOR_DATA::updateSTempAndSHumi(){
  // GET SOIL TEMP AND HUMI DATA
  Serial.println("Update to soil temp and humi with data...");
  Serial2.write(data.getRequestsTempsHumi(), 8);
  delay(1000);
  if (Serial2.available()) {
    uint8_t receivedData[9];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    sHumi = ((int)receivedData[3]*256 + (int)receivedData[4])/10.0;
    sTemp = ((int)receivedData[5]*256 + (int)receivedData[6])/10.0;
  }
};
void SENSOR_DATA::updateSpH(){
  // GET SOIL PH DATA
  Serial.println("Update to soil pH with data...");
  Serial2.write(data.getRequestspH(), 8);
  delay(1000);
  if (Serial2.available()) {
    uint8_t receivedData[9];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    spH = ((uint16_t)(receivedData[3] << 8) | (uint16_t)receivedData[4]) * 0.01;
  }
};
void SENSOR_DATA::updateSNPK(){
  // GET SOIL NPK DATA
  Serial.println("Update to soil NPK with data...");
  Serial2.write(data.getRequestsNPK(), 8);
  delay(1000);
  if (Serial2.available()) {
    uint8_t receivedData[11];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    sNito = ((uint16_t)(receivedData[3] << 8) | (uint16_t)receivedData[4]);
    sPhotho = (uint16_t)(receivedData[5] << 8) | (uint16_t)receivedData[6];
    sKali = (uint16_t)(receivedData[7] << 8) | (uint16_t)receivedData[8];
  }
};
void SENSOR_DATA::updateSConductivity(){
  // GET SOIL Conductivity DATA
  Serial.println("Update to soil conductivity with data...");
  Serial2.write(data.getRequestsConductivity(), 8);
  delay(1000);
  if (Serial2.available()) {
    uint8_t receivedData[7];
    Serial2.readBytes(receivedData, sizeof(receivedData));
    sConductivity = (uint16_t)(receivedData[3] << 8) | (uint16_t)receivedData[4];
  }
};

AllSensorData SENSOR_DATA::getAllData(){
  updateTempAndHumi();
  updateSTempAndSHumi();
  updateSpH();
  updateSNPK();
  updateSConductivity();

  AllSensorData tempData;
  tempData.temp = temp;
  tempData.humi = humi;
  tempData.spH = spH;
  tempData.sTemp = sTemp;
  tempData.sHumi = sHumi;
  tempData.sNito = sNito;
  tempData.sPhotho = sPhotho;
  tempData.sKali = sKali;
  tempData.sConductivity = sConductivity;

  return tempData;
};

String SENSOR_DATA::floatToString(float value) {
  char buffer[20];  // Đủ lớn để chứa chuỗi
  sprintf(buffer, "%.2f", value);
  return String(buffer);
}

/*.........................Implement SENSOR_RS485 class............................*/

SENSOR_RS485::SENSOR_RS485(){
  request_temp_humi = new uint8_t[8]{0x14, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC6, 0xCE};
  request_sTemp_sHumi = new uint8_t[8]{0x01, 0x03, 0x00, 0x12, 0x00, 0x02, 0x64, 0x0E};
  request_spH = new uint8_t[8]{0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B};
  request_sNPK = new uint8_t[8]{0x01, 0x03, 0x00, 0x1E, 0x00, 0x03, 0x65, 0xCD};
  request_sConductivity = new uint8_t[8]{0x01, 0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xCE};
};

SENSOR_RS485::~SENSOR_RS485() {
    delete[] request_temp_humi;
    delete[] request_sTemp_sHumi;
    delete[] request_spH;
    delete[] request_sNPK;
    delete[] request_sConductivity;
};

// Hàm để lấy con trỏ tới giá trị nhiệt độ và độ ẩm 
uint8_t* SENSOR_RS485::getRequestTempHumi() {
    return request_temp_humi;
}

// Hàm để lấy con trỏ tới chuỗi request nhiệt độ và độ ẩm đất
uint8_t* SENSOR_RS485::getRequestsTempsHumi() {
    return request_sTemp_sHumi;
}

// Hàm để lấy con trỏ tới chuỗi request pH
uint8_t* SENSOR_RS485::getRequestspH() {
    return request_spH;
}

// Hàm để lấy con trỏ tới chuỗi request NPK
uint8_t* SENSOR_RS485::getRequestsNPK() {
    return request_sNPK;
}

// Hàm để lấy con trỏ tới chuỗi request Conductivity
uint8_t* SENSOR_RS485::getRequestsConductivity() {
    return request_sConductivity;
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
  sensor3["id"] = "temp_0002";
  sensor3["value"] = round(data.sTemp * 100) / 100.0;

  JsonObject sensor4 = sensors.createNestedObject();
  sensor4["id"] = "humi_0002";
  sensor4["value"] = round(data.sHumi * 100) / 100.0;

  JsonObject sensor5 = sensors.createNestedObject();
  sensor5["id"] = "ph_0002";
  sensor5["value"] = round(data.pH * 100) / 100.0;

  JsonObject sensor6 = sensors.createNestedObject();
  sensor6["id"] = "EC_0002";
  sensor6["value"] = round(data.sConductivity * 100) / 100.0;

  JsonObject sensor7 = sensors.createNestedObject();
  sensor7["id"] = "Nito_0002";
  sensor7["value"] = round(data.sNito * 100) / 100.0;

  JsonObject sensor8 = sensors.createNestedObject();
  sensor8["id"] = "Photpho_0002";
  sensor8["value"] = round(data.sPhotho * 100) / 100.0;

  JsonObject sensor9 = sensors.createNestedObject();
  sensor9["id"] = "Kali_0002";
  sensor9["value"] = round(data.sKali * 100) / 100.0;

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