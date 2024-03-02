/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with ATOM DTU NB MQTT Client sample source code
* Visit the website for more
information：https://docs.m5stack.com/en/atom/atom_dtu_nb
* describe: ATOM DTU NB MQTT Clien Example.
* Libraries:
    - [TinyGSM - modify](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/arduino/lib/TinyGSM.zip)
    - [PubSubClient](https://github.com/knolleary/pubsubclient.git)
* date：2022/4/14
*******************************************************************************
*/

#ifndef INC_NBIOT_H_
#define INC_NBIOT_H_

#include <ArduinoJson.h>
#include <M5Atom.h>
#include "sensor_data.h"
#include "ATOM_DTU_NB.h"
#include <PubSubClient.h>
#include <TinyGsmClient.h>
#include <time.h>
#include <sys/time.h>
#include <ThingsBoard.h>

#define MQTT_BROKER   "demo.thingsboard.io"
#define MQTT_PORT     1883
#define MQTT_USERNAME "haole1110"

#define MQTT_PASSWORD "11102002"
// #define MQTT_D_TOPIC  "TestAtom"
#define MQTT_U_TOPIC  "v1/devices/me/attributes"  //  上传数据主题

#define UPLOAD_INTERVAL 10000
// uint32_t lastReconnectAttempt = 0;

// TinyGsm modem(SerialAT, ATOM_DTU_SIM7020_RESET);

// TinyGsmClient tcpClient(modem);
// PubSubClient mqttClient(MQTT_BROKER, MQTT_PORT, tcpClient);

bool mqttConnect(void);
void nbConnect(void);
void SetupNbiot();
void SendData(const char* PubData);



#endif
