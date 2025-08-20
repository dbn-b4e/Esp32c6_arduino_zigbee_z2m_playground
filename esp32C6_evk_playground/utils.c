#include <Arduino.h>

/********************* RGB LED functions **************************/
void setLED(uint8_t pin, bool value)
{
  digitalWrite(pin, value);
}

/********************* RGB LED functions **************************/
void setRelay(uint8_t pin, bool value)
{
  digitalWrite(pin, value);
}