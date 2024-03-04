#include "nbiot.h"

uint32_t lastReconnectAttempt = 0;  //Save time last connect to MQTT server

/*Initializing object client to use NBIoT network*/
TinyGsm modem(SerialAT, ATOM_DTU_SIM7020_RESET);
TinyGsmClient tcpClient(modem);
PubSubClient mqttClient(MQTT_BROKER, MQTT_PORT, tcpClient);


/*.....................FUNCTION..........................*/

void log(String info) {
    SerialMon.println(info);
}


void nbConnect(void) {
    unsigned long start = millis();
    log("Initializing modem...");
    while (!modem.init()) {
        log("waiting...." + String((millis() - start) / 1000) + "s");
    };

    start = millis();
    log("Waiting for network...");
    while (!modem.waitForNetwork()) {
        log("waiting...." + String((millis() - start) / 1000) + "s");
    }
    log("success");
}

void SetupNbiot() {
    M5.begin(true, false, true);
    Serial.println(">>ATOM DTU NB MQTT TEST");
    SerialAT.begin(SIM7020_BAUDRATE, SERIAL_8N1, ATOM_DTU_SIM7020_RX,
                   ATOM_DTU_SIM7020_TX);
    M5.dis.fillpix(0x0000ff);
    nbConnect();
}


bool mqttConnect(void) {
    log("Connecting to ");
    log(MQTT_BROKER);
    // Connect to MQTT Broker
    String mqttid = ("MQTTID_" + String(random(65536)));
    bool status =
        mqttClient.connect(mqttid, MQTT_USERNAME, MQTT_PASSWORD);
    if (status == false) {
        log(" fail");
        return false;
    }
    log("MQTT CONNECTED!");
    if (mqttClient.subscribe(MQTT_U_TOPIC)){
      Serial.println("Subcribed to Topic");
    } else {
      Serial.println("Not subcribed to Topic");
    }
    return mqttClient.connected();
}


void SendData(const char* PubData) {
    static unsigned long timer = 0;

    if (!mqttConnect()) {
        log(">>MQTT NOT CONNECTED");
        // Reconnect every 10 seconds
        M5.dis.fillpix(0xff0000);
        uint32_t t = millis();
        if (t - lastReconnectAttempt > 3000L) {
            lastReconnectAttempt = t;
            if (mqttConnect()) {
                lastReconnectAttempt = 0;
            }
        }
        delay(100);
    }
    if (millis() >= timer) {
        timer = millis() + UPLOAD_INTERVAL;
        if (mqttClient.publish(MQTT_U_TOPIC, PubData)){
          Serial.println("PUBLISH COMPLETED!!");
        } else {
          Serial.println("NOT PUBLISH!!");
        }
    }
    M5.dis.fillpix(0x00ff00);
    mqttClient.loop();
}


