/* Begin user private include */
  #include <M5Atom.h>
  #include "sensor_data.h"
  #include "nbiot.h"
  #include <SimpleKalmanFilter.h>
  #include "relay.h"
/* End user private include */

  AllSensorData TempData;
  SENSOR_DATA DataSensor;
  Relay AllRelay;
  SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.01);

/*  Setup and loop function */
  void setup() {
    M5.begin(true, false, true);                                    // M5 ATOM system init
    Serial2.begin(9600, SERIAL_8N1, 33, 23);                        // Init port for RS485 protocol    
    RL_Init(25, 21);
    SetupNbiot();
  }

  void loop() {
    M5.update();
    RL_SetState(0, 1);
    AllRelay.rl1 = true;
    delay(5000);
    TempData = DataSensor.getAllData();
    Serial.println(ConvertToJsonObject(TempData, AllRelay));
    SendData(ConvertToJsonObject(TempData, AllRelay));
    delay(2000);
    RL_SetState(0, 0);
    delay(600000);
  }
/* End etup and loop function */

