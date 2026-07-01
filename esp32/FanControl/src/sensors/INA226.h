#pragma once

void ina226Begin();

float readVoltage12();
float readVoltage19();

float readCurrent12();
float readCurrent19();

float readPower12();
float readPower19();