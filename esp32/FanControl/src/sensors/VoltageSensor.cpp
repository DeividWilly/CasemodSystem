#include <Arduino.h>

#if FAKE_SENSOR_VOLTS

void ina226Begin() {

}

float readVoltage12() {
    static float v = 12.0f;
    v += random(-10, 10) / 100.0f;
    return v;
}

float readVoltage19() {
    static float v = 19.0f;
    v += random(-10, 10) / 100.0f;
    return v;
}

float readCurrent12() {
    static float i = 1.5f;
    i += random(-5, 5) / 100.0f;
    return i;
}

float readCurrent19() {
    static float i = 3.2f;
    i += random(-5, 5) / 100.0f;
    return i;
}

float readPower12() {
    return readVoltage12() * readCurrent12();
}

float readPower19() {
    return readVoltage19() * readCurrent19();
}

#else

#include <Wire.h>
#include <INA226.h>
#include "INA226.h"

#include "../config/Pins.h"

static INA226 ina12(0x40);
static INA226 ina19(0x41);

void ina226Begin() {
    Wire.begin(Pins::INA226_SDA, Pins::INA226_SCL);

    ina12.begin();
    ina12.setMaxCurrentShunt(10.0, 0.002);

    ina19.begin();
    ina19.setMaxCurrentShunt(10.0, 0.002);
}

float readVoltage12() {
    return ina12.getBusVoltage();
}

float readVoltage19() {
    return ina19.getBusVoltage();
}

float readCurrent12() {
    return ina12.getCurrent();
}

float readCurrent19() {
    return ina19.getCurrent();
}

float readPower12() {
    return ina12.getPower();
}

float readPower19() {
    return ina19.getPower();
}

#endif