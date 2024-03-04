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

/* Begin user private include */
#include <ArduinoJson.h>
#include <M5Atom.h>
#include "sensor_data.h"
#include "ATOM_DTU_NB.h"
#include <PubSubClient.h>
#include <TinyGsmClient.h>
#include <time.h>
#include <sys/time.h>
/* End user private include */

/*Define MQTT information*/
#define MQTT_BROKER   "mqttserver.tk"
#define MQTT_PORT     1883
#define MQTT_USERNAME "innovation"
#define MQTT_PASSWORD "Innovation_RgPQAZoA5N"
#define MQTT_U_TOPIC  "/innovation/airmonitoring/NBIOTs"


#define UPLOAD_INTERVAL 10000


bool mqttConnect(void); 
void nbConnect(void);
void SetupNbiot();
void SendData(const char* PubData);



#endif
