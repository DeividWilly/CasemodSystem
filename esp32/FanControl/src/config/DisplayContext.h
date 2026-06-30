#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

extern SemaphoreHandle_t displayMutex;

constexpr int HEADER_Y = 23;

constexpr int VALUE_TOP = 4;
constexpr int VALUE_H   = 18;

constexpr int CPU_LABEL_X  = 1;
constexpr int CPU_VALUE_X  = 50;

constexpr int LOAD_LABEL_X = 107;
constexpr int LOAD_VALUE_X = 165;

constexpr int RAM_LABEL_X  = 1;
constexpr int RAM_VALUE_X  = 50;

constexpr int FAN_LABEL_X = 240;
constexpr int FAN_VALUE_X = 295;

constexpr int FONT_TEMP_VALUE_X = 360;

constexpr int HEADER_H = 30;

constexpr int FOOTER_H = 1;
constexpr int FOOTER_Y = 240;
