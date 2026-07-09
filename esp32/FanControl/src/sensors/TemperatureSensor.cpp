#include <Arduino.h>
#if FAKE_SENSOR_TEMP

void ds18b20(){

}

float readTemperature() {
    static float temp = 40.0f;
    temp += ((random(-5, 5)) / 10.0f);
    return temp;
}

#else

#include "DS18B20.h"

#include <OneWire.h>
#include <DallasTemperature.h>

#include "../config/Pins.h"

static OneWire oneWire(Pins::DS18B20_PIN);

static DallasTemperature sensors(&oneWire);

void ds18b20begin() {
    sensors.begin();
    Serial.print("Sensors: ");
    Serial.println(sensors.getDeviceCount());
}

float readTemperature() {
    
    sensors.requestTemperatures();
    return sensors.getTempCByIndex(0);
}

#endif