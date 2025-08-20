// Copyright 2025 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @brief This example demonstrates Zigbee multistate input / output device.
 *
 * The example demonstrates how to use Zigbee library to create a end device multistate device.
 * In the example, we have two multistate devices:
 * - zbMultistateDevice: uses defined application states from Zigbee specification
 * - zbMultistateDeviceCustom: uses custom application states (user defined)
 * 
 * Proper Zigbee mode must be selected in Tools->Zigbee mode
 * and also the correct partition scheme must be selected in Tools->Partition Scheme.
 *
 * Please check the README.md for instructions and more detailed description.
 *
 * Created by Jan Procházka (https://github.com/P-R-O-C-H-Y/)
 * Modified by Pat Clay
 */

#define LOOPTASK_STACK_SIZE (64 * 1024)
//SET_LOOP_TASK_STACK_SIZE(12*1024);
#define CONFIG_ARDUINO_LOOP_STACK_SIZE     (16*1024)       // 16KB

#ifndef ZIGBEE_MODE_ZCZR
#error "Zigbee coordinator/router device mode is not selected in Tools->Zigbee mode"
#endif

#include <Arduino.h>
#include "Zigbee.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "CommandParser.h" // https://github.com/sivar2311/CommandParser/
#include <jled.h>
#include <string>

enum string_code_e {
    eZb_reset = 1,
    eBrd_reset
};
// typedef string_code_e string_code_t;


//string_code_t hashit();

/* Zigbee multistate device configuration */
#define MULTISTATE_DEVICE_ENDPOINT_NUMBER 1
#define MULTISTATE_DEVICE_CUSTOM_ENDPOINT_NUMBER 2
#define TEMP_SENSOR_ENDPOINT_NUMBER 3
#define TEMP_HUMIDITY_SENSOR_ENDPOINT_NUMBER 4
#define ZIGBEE_OUTLET_ENDPOINT 5

#define ZB_OUTLET_ENABLE 

uint8_t button = BOOT_PIN;
#define ONE_WIRE_BUS  4     // GPIO For OW
#define DHTPIN        21    // Digital pin connected to the DHT sensor 
uint8_t led = 2;            // Use custom LED pin
uint8_t relay_pin = 19;     // Relay pin
static const uint8_t   LED2_PIN = 3;
static const uint8_t   LED3_PIN = 23;

// JLed sequencer
//JLedSequence sequence(JLedSequence::eMode::SEQUENCE, leds);
auto led_test = JLed(LED2_PIN);
auto led_zb = JLed(LED3_PIN);

// Tasks handles
TaskHandle_t tsk_console_task;
TaskHandle_t tsk_task1_task;
TaskHandle_t tsk_led_task;

// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
#define DHTTYPE    DHT21     // DHT 21 (AM2301)
uint32_t delayMS;

// Optional Time cluster variables
struct tm timeinfo;
struct tm *localTime;
int32_t timezone;

// Dallas 1-wire DS18B20
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Command parser
CommandParser cp(Serial);

// DHT21 (AM2301)
DHT_Unified dht(DHTPIN, DHTTYPE);
sensor_t sensor;
sensors_event_t event;

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;

// zbMultistateDevice will use defined application states
ZigbeeMultistate zbMultistateDevice = ZigbeeMultistate(MULTISTATE_DEVICE_ENDPOINT_NUMBER);

// zbMultistateDeviceCustom will use custom application states (user defined)
ZigbeeMultistate zbMultistateDeviceCustom = ZigbeeMultistate(MULTISTATE_DEVICE_CUSTOM_ENDPOINT_NUMBER);

// zbTempSensor simple temperature sensor
ZigbeeTempSensor zbTempSensor = ZigbeeTempSensor(TEMP_SENSOR_ENDPOINT_NUMBER);

// zbTempSensor humidity and temperature sensor
ZigbeeTempSensor zbTempHumiditySensor = ZigbeeTempSensor(TEMP_HUMIDITY_SENSOR_ENDPOINT_NUMBER);

// ZigbeePowerOutlet
#ifdef ZB_OUTLET_ENABLE 
ZigbeePowerOutlet zbOutlet = ZigbeePowerOutlet(ZIGBEE_OUTLET_ENDPOINT);
#endif

const char* multistate_custom_state_names[6] = {"Off", "On", "UltraSlow", "Slow", "Fast", "SuperFast"};

/********************* RGB LED functions **************************/
void setLED(bool value)
{
  digitalWrite(led, value);
  digitalWrite(relay_pin, value);
}

/********************* RGB LED functions **************************/
void setRelay(bool value)
{
  digitalWrite(relay_pin, value);
}

void handleHello(const std::vector<String>& params)
{
    if (params.size() > 2) {
        Serial.printf("Hello %s! Nice to meet you!\r\n", params[1].c_str());
    } else {
        Serial.println("Hello unknown!");
    }
}

void handle_meminfo(const std::vector<String>& params)
{

    IPAddress ip;
    if (ip.fromString(params[1])) {
        Serial.println(ip);
    } else {
        Serial.println("Invalid IP address!");
    }
}

void handleTemp(const std::vector<String>& params)
{
  if (params.size() < 2) {
    Serial.println("Please provide a temperature as parameter");
    return;
  }

  float temperature = params[1].toFloat();
  Serial.printf("Temperature set to %f\r\n", temperature);
}

void handleCount(const std::vector<String>& params)
{
    if (params.size() < 3)
    {
        Serial.println("Count <from> <to>");
        return;
    }

    size_t from = params[1].toInt();
    size_t to = params[2].toInt();

    for (size_t i = from; i<=to; i++)
    {
        Serial.println(i);
    }
}




enum string_code_e hashit (std::string const& inString)
{
    if (inString == "zigbee") return eZb_reset;
    if (inString == "board") return eBrd_reset;
}



void handleReset(const std::vector<String>& params)
{
  if (params.size() < 2)
  {
    Serial.println("Usage: 'reset zigbee' / 'reset board' ");
    return;
  }

  switch (hashit(params[1].c_str()))
  {
    case eZb_reset:
        Serial.println("Resetting Zigbee to factory and rebooting in 1s.");
        delay(1000);
        Zigbee.factoryReset();
      break;
    case eBrd_reset:
      Serial.println("Resetting board");
      break;
    default:
      Serial.println("Invalid commandd");
      break;
  }
}



void handleNotFound(const std::vector<String>& params) {
    Serial.printf("Command \"%s\" is not implemented!\r\n", params[0].c_str());
}

void handleOverflow() {
    Serial.println("Buffer overflow!");
}

void onStateChange(uint16_t state) {
  // print the state
  Serial.printf("Received state change: %d\r\n", state);
  // print the state name using the stored state names
  const char* const* state_names = zbMultistateDevice.getMultistateOutputStateNames();
  if (state_names && state < zbMultistateDevice.getMultistateOutputStateNamesLength()) {
    Serial.printf("State name: %s\r\n", state_names[state]);
  }
  // print state index of possible options
  Serial.printf("State index: %d / %d\r\n", state, zbMultistateDevice.getMultistateOutputStateNamesLength() - 1);
}

void onStateChangeCustom(uint16_t state) {
  // print the state
  Serial.printf("Received state change: %d\r\n", state);
  // print the state name using the stored state names
  const char* const* state_names = zbMultistateDeviceCustom.getMultistateOutputStateNames();
  if (state_names && state < zbMultistateDeviceCustom.getMultistateOutputStateNamesLength()) {
    Serial.printf("State name: %s\r\n", state_names[state]);
  }
  // print state index of possible options
  Serial.printf("State index: %d / %d\r\n", state, zbMultistateDevice.getMultistateOutputStateNamesLength() - 1);

  Serial.print("Changing to fan mode to: ");
  switch (state) {
    case 0:
      Serial.println("Off");
      break;
    case 1:
      Serial.println("On");
      break;
    case 2:
      Serial.println("Slow");
      break;
    case 3:
      Serial.println("Medium");
      break;
    case 4:
      Serial.println("Fast");
      break;
    default:
      Serial.println("Invalid state");
      break;
  }
}


void setup() {
  Serial.begin(115200);
  
  Serial.printf("Arduino Stack was set to %d bytes", getArduinoLoopTaskStackSize());

    // Print unused stack for the task that is running setup()
  Serial.printf("\nSetup() - Free Stack Space: %d", uxTaskGetStackHighWaterMark(NULL));

  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);

  // AM2301 sensor
  dht.begin();


  

  
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));

  delayMS = sensor.min_delay / 1000;


  Serial.print (F("DHT Minimum poll time:  ")); Serial.print(delayMS);

  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }

  // Locate devices on the bus
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: ");
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  // Assign address manually. The addresses below will need to be changed
  // to valid device addresses on your bus. Device address can be retrieved
  // by using either oneWire.search(deviceAddress) or individually via
  // sensors.getAddress(deviceAddress, index)
  // Note that you will need to use your specific address here
  //insideThermometer = { 0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF0 };

  // Method 1:
  // Search for devices on the bus and assign based on an index. Ideally,
  // you would do this to initially discover addresses on the bus and then
  // use those addresses and manually assign them (see above) once you know
  // the devices on your bus (and assuming they don't change).
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0");

  // method 2: search()
  // search() looks for the next device. Returns 1 if a new address has been
  // returned. A zero might mean that the bus is shorted, there are no devices,
  // or you have already retrieved all of them. It might be a good idea to
  // check the CRC to make sure you didn't get garbage. The order is
  // deterministic. You will always get the same devices in the same order
  //
  // Must be called before search()
  //oneWire.reset_search();
  // assigns the first address found to insideThermometer
  //if (!oneWire.search(insideThermometer)) Serial.println("Unable to find address for insideThermometer");

  // show the addresses we found on the bus
  Serial.print("Device 0 Address: ");
  printAddress(insideThermometer);
  Serial.println();

  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensors.setResolution(insideThermometer, 12);
  Serial.print("Device 0 Resolution: ");
  sensors.setResolution(12);
  Serial.print(sensors.getResolution(insideThermometer), DEC);
  Serial.println();
  // Init button switch
  pinMode(button, INPUT_PULLUP);

  // Optional: set Zigbee device name and model
  zbMultistateDevice.setManufacturerAndModel("Espressif", "ZigbeeMultistateDevice");
  zbMultistateDevice.setPowerSource(ZB_POWER_SOURCE_MAINS);
  //zbTempSensor.setPowerSource(ZB_POWER_SOURCE_BATTERY, 100, 35);

  // Set callback function for power outlet change
#ifdef ZB_OUTLET_ENABLE 
  zbOutlet.onPowerOutletChange(setLED);
  //zbOutlet.onPowerOutletChange(setRelay);
#endif

  // Setup simple temperature sensor
  zbTempSensor.setMinMaxValue(10, 50);
  zbTempSensor.setTolerance(0.1);
  zbTempSensor.addTimeCluster();
  zbTempSensor.setPowerSource(ZB_POWER_SOURCE_MAINS);

   // Setup temperature & humidity sensor
  zbTempHumiditySensor.setMinMaxValue(10, 50);
  zbTempHumiditySensor.setTolerance(0.1);
  zbTempHumiditySensor.addTimeCluster();
  // Add humidity cluster to the temperature sensor device with min, max and tolerance values
  zbTempHumiditySensor.addHumiditySensor(0, 100, 1);
  zbTempHumiditySensor.setPowerSource(ZB_POWER_SOURCE_MAINS);


  // Set up analog input
  zbMultistateDevice.addMultistateInput();
  zbMultistateDevice.setMultistateInputApplication(ZB_MULTISTATE_APPLICATION_TYPE_0_INDEX);
  zbMultistateDevice.setMultistateInputDescription("Fan (on/off/auto)");
  zbMultistateDevice.setMultistateInputStates(ZB_MULTISTATE_APPLICATION_TYPE_0_STATE_NAMES, ZB_MULTISTATE_APPLICATION_TYPE_0_NUM_STATES);

  // Set up analog output
  zbMultistateDevice.addMultistateOutput();
  zbMultistateDevice.setMultistateOutputApplication(ZB_MULTISTATE_APPLICATION_TYPE_7_INDEX);
  zbMultistateDevice.setMultistateOutputDescription("Light (high/normal/low)");
  zbMultistateDevice.setMultistateOutputStates(ZB_MULTISTATE_APPLICATION_TYPE_7_STATE_NAMES, ZB_MULTISTATE_APPLICATION_TYPE_7_NUM_STATES);

  // Set up custom output
  zbMultistateDeviceCustom.addMultistateOutput();
  zbMultistateDeviceCustom.setMultistateOutputApplication(ZB_MULTISTATE_APPLICATION_TYPE_OTHER_INDEX);
  zbMultistateDeviceCustom.setMultistateOutputDescription("Fan (on/off/slow/medium/fast)");
  zbMultistateDeviceCustom.setMultistateOutputStates(multistate_custom_state_names, 5);

  // Set callback function for multistate output change 
  zbMultistateDevice.onMultistateOutputChange(onStateChange);
  zbMultistateDeviceCustom.onMultistateOutputChange(onStateChangeCustom);

  // Add endpoints to Zigbee Core
  Zigbee.addEndpoint(&zbMultistateDevice);
  Zigbee.addEndpoint(&zbMultistateDeviceCustom);
  Zigbee.addEndpoint(&zbTempSensor);
  Zigbee.addEndpoint(&zbTempHumiditySensor);

#ifdef ZB_OUTLET_ENABLE   
  Zigbee.addEndpoint(&zbOutlet);
#endif

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

  // Nex task for loop with bigger stack size
  xTaskCreateUniversal(newloop,"newloop", 64*1024, NULL, 3, &tsk_task1_task, ARDUINO_RUNNING_CORE);
 // vTaskDelete(NULL);

  // Set reporting interval for temperature measurement in seconds, must be called after Zigbee.begin()
  // min_interval and max_interval in seconds, delta (temp change in 0,1 °C)
  // if min = 1 and max = 0, reporting is sent only when temperature changes by delta
  // if min = 0 and max = 10, reporting is sent every 10 seconds or temperature changes by delta
  // if min = 0, max = 10 and delta = 0, reporting is sent every 10 seconds regardless of temperature change
  zbTempSensor.setReporting(10, 3600, 0.1);
  zbTempHumiditySensor.setReporting(10, 3600, 0.1);
  zbTempHumiditySensor.setHumidityReporting(10, 3600, 0.1);
  

  // Initialise command parser callbacks
    cp.onCommand("hello", handleHello, "[name|string (optional)]");
    //cp.onCommand("temp", handleTemp, "[temperature|number]", CommandParser::CaseSensivity::IGNORE);
    cp.onCommand("count", handleCount, "[from|number] [to|number]");
    //cp.onCommand("reset", handleReset, "[zigbee or board]", CommandParser::CaseSensivity::IGNORE);
    cp.onOverflow(handleOverflow);
    cp.onNotFound(handleNotFound);

  // Terminal console task
  xTaskCreateUniversal(tsk_console,"tsk_console", 16*1024, NULL, 2, &tsk_console_task, ARDUINO_RUNNING_CORE);

  // JLede task
  xTaskCreateUniversal(tsk_led,"tsk_led", 8*1024, NULL, 1, &tsk_led_task, ARDUINO_RUNNING_CORE);

//  vTaskDelete(NULL);
  
}

void newloop(void* param) {
  while(1)
  {
    static uint32_t timeCounter = 0;

    if(!Zigbee.connected())
    {
      if(!led_zb.IsRunning())
        led_zb.Blink(100,100).Repeat(1000);
    }
    else
    {
      if(!led_zb.IsRunning())
        led_zb.Blink(1000,1000).Repeat(1000);
    }

    //delay(10000);
    
    // Checking button for factory reset and reporting
    if (digitalRead(button) == LOW)
    {  // Push button pressed
      Serial.println("Button is pressed");
      // Key debounce handling
      delay(100);
      int startTime = millis();
      while (digitalRead(button) == LOW)
      {
        delay(50);
        if ((millis() - startTime) > 3000)
        {
          // If key pressed for more than 3secs, factory reset Zigbee and reboot
          Serial.println("Resetting Zigbee to factory and rebooting in 1s.");
          delay(1000);
          Zigbee.factoryReset();
        }
      }
    led_test.Blink(250,250).Repeat(10);

    Serial.printf("Taks1 Mem: %d\n", uxTaskGetStackHighWaterMark(tsk_task1_task));
    Serial.printf("Taks2 Mem: %d\n", uxTaskGetStackHighWaterMark(tsk_console_task));

      // Toggle state by pressing the button
     #ifdef ZB_OUTLET_ENABLE 
      zbOutlet.setState(!zbOutlet.getPowerOutletState());
    #endif

      // Force temperature report
      zbTempSensor.reportTemperature();
      zbTempHumiditySensor.reportTemperature();
      zbTempHumiditySensor.reportHumidity();      

      // For demonstration purposes, increment the multistate output/input value by 1
      if (zbMultistateDevice.getMultistateOutput() < zbMultistateDevice.getMultistateOutputStateNamesLength() - 1) {
        zbMultistateDevice.setMultistateOutput(zbMultistateDevice.getMultistateOutput() + 1);
        zbMultistateDevice.reportMultistateOutput();
        zbMultistateDevice.setMultistateInput(zbMultistateDevice.getMultistateOutput());
        zbMultistateDevice.reportMultistateInput();
      } else {
        zbMultistateDevice.setMultistateOutput(0);
        zbMultistateDevice.reportMultistateOutput();
        zbMultistateDevice.setMultistateInput(zbMultistateDevice.getMultistateOutput());
        zbMultistateDevice.reportMultistateInput();
      }

      if (zbMultistateDeviceCustom.getMultistateOutput() < zbMultistateDeviceCustom.getMultistateOutputStateNamesLength() - 1) {
        zbMultistateDeviceCustom.setMultistateOutput(zbMultistateDeviceCustom.getMultistateOutput() + 1);
        zbMultistateDeviceCustom.reportMultistateOutput();
      } else {
        zbMultistateDeviceCustom.setMultistateOutput(0);
        zbMultistateDeviceCustom.reportMultistateOutput();
      }
    }
    delay(10);

 

    #if 0
      Serial.println(ESP.getHeapSize());
      Serial.println(ESP.getFreeHeap());
      Serial.println(ESP.getMinFreeHeap());
      Serial.println(ESP.getMaxAllocHeap());
      Serial.println(uxTaskGetStackHighWaterMark(NULL));
    #endif
    }
}

void tsk_led(void* param) {
  while(1)
  {
    led_test.Update();
    led_zb.Update();
    delay(1);
  }
}

void tsk_console(void* param) {
  while(1)
  {
  static bool TaskStarted = false;
  if (TaskStarted == false)
  {
    TaskStarted = true;
    Serial.println("Terminal task started");
  }

  cp.processFromStream(Serial);

  delay(10);

  }
}

void loop()
{
  vTaskDelete(NULL);
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

// function to print the temperature for a device
float ConvertTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);

  if (tempC == DEVICE_DISCONNECTED_C)
  {
    Serial.println("Error: Could not read temperature data");
    return tempC;
  }
}
// function to print the temperature for a device
float printTemperature(DeviceAddress deviceAddress)
{
  // method 2 - faster
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == DEVICE_DISCONNECTED_C)
  {
    //Serial.println("Error: Could not read temperature data");
    return 0;
  }
  else
  {
  //Serial.print("Temp C: ");
  //Serial.print(tempC);
  //Serial.print(" Temp F: ");
  //Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
  return tempC;
  }
}


/************************ Temp sensor *****************************/
static void temp_sensor_value_update(void *arg) {
  for (;;)
  {
    // call sensors.requestTemperatures() to issue a global temperature
    // request to all devices on the bus
    Serial.print("Requesting temperatures...");
    sensors.requestTemperatures(); // Send the command to get temperatures
    Serial.println("DONE");
    delay(1500);
    //printTemperature(insideThermometer); // Use a simple function to print out the data
    //Serial.print(printTemperature(insideThermometer));
    
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      Serial.println(F("DHT Error reading temperature!"));
    }
    else {
      float dht_temp = event.temperature;
      Serial.print(F("DHT Temperature: "));
      Serial.print(dht_temp);
      Serial.println(F("°C"));
      zbTempHumiditySensor.setTemperature(dht_temp);
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.println(F("DHT Error reading humidity!"));
    }
    else {
      float dht_humidity = event.relative_humidity;
      Serial.print(F("DHT Humidity: "));
      Serial.print(dht_humidity);
      Serial.println(F("%"));
      zbTempHumiditySensor.setHumidity(dht_humidity);
    }

    float tsens_value_ow = printTemperature(insideThermometer); // Use a simple function to print out the data
    Serial.printf("Updated DS18B20 temperature sensor value to %.2f°C\r\n", tsens_value_ow);
    zbTempSensor.setTemperature(tsens_value_ow);

    // Read temperature sensor value
    //float tsens_value = temperatureRead();
    //Serial.printf("Updated temperature sensor value to %.2f°C\r\n", tsens_value);
    // Update temperature value in Temperature sensor EP
    //zbTempSensor.setTemperature(tsens_value);
    
    delay(1000);
  }
}