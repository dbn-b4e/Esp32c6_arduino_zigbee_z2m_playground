#include <Arduino.h>
#include "GuiManager.h"
#include "ui.h"
#include "custom_actions.h"
#include "Zigbee.h"

#define TEMP_SENSOR_ENDPOINT_NUMBER 1
TaskHandle_t tsk_temp_task;

// Optional Time cluster variables
struct tm timeinfo;
struct tm *localTime;
int32_t timezone;

// zbTempSensor simple temperature sensor
ZigbeeTempSensor zbTempSensor = ZigbeeTempSensor(TEMP_SENSOR_ENDPOINT_NUMBER);

GuiManager gui;

/************************ Temp sensor *****************************/
static void temp_sensor_value_update(void *arg) {
  for (;;)
  {
    // call sensors.requestTemperatures() to issue a global temperature
    // request to all devices on the bus
    Serial.print("Requesting temperatures...");

    zbTempSensor.setTemperature(12.34);

    // Read temperature sensor value
    //float tsens_value = temperatureRead();
    //Serial.printf("Updated temperature sensor value to %.2f°C\r\n", tsens_value);
    // Update temperature value in Temperature sensor EP
    //zbTempSensor.setTemperature(tsens_value);
    
    delay(1000);
  }
}


void setup() {
    Serial.begin(115200);
    Serial.printf("Arduino Stack was set to %d bytes", getArduinoLoopTaskStackSize());
    Serial.printf("\nSetup() - Free Stack Space: %d", uxTaskGetStackHighWaterMark(NULL));

    // Optional: set Zigbee device name and model
    zbTempSensor.setManufacturerAndModel("B4E SRL", "Zigbee-lvgl-1");
    zbTempSensor.setMinMaxValue(10, 50);
    zbTempSensor.setTolerance(0.1);
    zbTempSensor.addTimeCluster();
    zbTempSensor.setPowerSource(ZB_POWER_SOURCE_MAINS);


    Zigbee.addEndpoint(&zbTempSensor);

  Serial.println("Starting Zigbee...");
  // When all EPs are registered, start Zigbee in Router Device mode
  if (!Zigbee.begin(ZIGBEE_ROUTER)) {
    Serial.println("Zigbee failed to start!");
    Serial.println("Rebooting...");
    ESP.restart();
  } else {
    Serial.println("Zigbee started successfully!");
  }
  
 #if 1
  Serial.println("Connecting to network");
  while (!Zigbee.connected()) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("Connected");
#endif

  // Optional: If time cluster is added, time can be read from the coordinator
  timeinfo = zbTempSensor.getTime();
  timezone = zbTempSensor.getTimezone();

  Serial.println("UTC time:");
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  time_t local = mktime(&timeinfo) + timezone;
  localTime = localtime(&local);

  Serial.println("Local time with timezone:");
  Serial.println(localTime, "%A, %B %d %Y %H:%M:%S");

    // Start Temperature sensor reading task
  xTaskCreate(temp_sensor_value_update, "temp_sensor_update", 4096, NULL, 10, NULL);

    gui.begin();

    // --- UI ---
    ui_init();

    gui.startLvglTask();


    Serial.println("Setup done");
}



void loop() {
    gui.update();

    static uint32_t last_update = 0;
    static uint8_t counter = 0;
    uint16_t analogVolts = analogReadMilliVolts(BAT_PIN);
    float vbat = analogVolts * 3.0 / 1000;
    char str_buffer[20];
    char uptime_str[16];

    if (millis() - last_update >= 1000) {
        last_update = millis();

        // Calculate uptime
        unsigned long secs = millis() / 1000;
        unsigned int hours = secs / 3600;
        unsigned int mins = (secs % 3600) / 60;
        unsigned int seconds = secs % 60;

        // Format uptime string
        snprintf(uptime_str, sizeof(uptime_str), "%02u:%02u:%02u", hours, mins, seconds);
        lv_label_set_text(objects.lbl_uptime, uptime_str);

        int progress = (counter++) % 100;
        sprintf(str_buffer, "%.1f V", vbat);

        lv_bar_set_value(objects.prg_1, progress, LV_ANIM_OFF);
        lv_label_set_text(objects.lbl_counter, String(progress).c_str());
        lv_label_set_text(objects.lbl_vbat, String(vbat).c_str());

    }
    delay(100);
}