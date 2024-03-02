/* Begin user private include */
  #include <M5Atom.h>
  #include "sensor_data.h"
  #include "nbiot.h"
  //#include "nbio2.cpp"
  #include <freertos/FreeRTOS.h>
  #include <freertos/task.h>
  #include <SimpleKalmanFilter.h>
/* End user private include */

  AllSensorData TempData;
  SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.01);

/*  Setup and loop function */
  void setup() {
    M5.begin(true, false, true);                                    // M5 ATOM system init
    Serial2.begin(9600, SERIAL_8N1, 33, 23);                        // Init port for RS485 protocol    
    // WatchdogInit();                                                 // Init watchdog                                       

    // /* FreeRTOS Begin adding tasks */
    // xTaskCreatePinnedToCore(taskGetData, "GetDataTask", 10000, NULL, 1, NULL, 0);         // TASK1
    // /* FreeRTOS End adding tasks */
    SetupNbiot();
    Serial.println(ConvertToJsonObject(TempData));

  }

  void loop() {
    M5.update();
    for (int i=0; i < 5; i++){
      TempData = GetAllSensorData(); 
      TempData.temp = simpleKalmanFilter.updateEstimate(TempData.temp);
      TempData.humi = simpleKalmanFilter.updateEstimate(TempData.humi);
      TempData.CO = simpleKalmanFilter.updateEstimate(TempData.CO);
      TempData.CO2 = simpleKalmanFilter.updateEstimate(TempData.CO2);
      TempData.SO2 = simpleKalmanFilter.updateEstimate(TempData.SO2);
      TempData.NO2 = simpleKalmanFilter.updateEstimate(TempData.NO2);
      TempData.PM25 = simpleKalmanFilter.updateEstimate(TempData.PM25);
      TempData.PM10 = simpleKalmanFilter.updateEstimate(TempData.PM10);
      TempData.O3 = simpleKalmanFilter.updateEstimate(TempData.O3);

    }
    Serial.println(ConvertToJsonObject(TempData));
    SendData(ConvertToJsonObject(TempData));
    delay(1000);
  }
/* End etup and loop function */
 

/* Begin private task
 * Including:
 *  + Watchdog init
 *  + resetModule
 *  + taskGetData
*/
  void IRAM_ATTR resetModule() {
    ets_printf("reboot\n");
    esp_restart();
  }

  // TickType_t lastDataTime = 0;
  // TickType_t waitTimeInTicks = pdMS_TO_TICKS(300000); // 5 minutes in TICKs
