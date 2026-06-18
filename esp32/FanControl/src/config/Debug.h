#pragma once

#include <Arduino.h>

#define DEBUG_SERIAL  1
#define DEBUG_DISPLAY 1
#define DEBUG_PWM     1

#if DEBUG_SERIAL
    #define SERIAL_LOG(fmt, ...) Serial.printf("[SERIAL] " fmt, ##__VA_ARGS__)
#else
    #define SERIAL_LOG(...)
#endif

#if DEBUG_DISPLAY
    #define DISPLAY_LOG(fmt, ...) Serial.printf("[DISPLAY] " fmt, ##__VA_ARGS__)
#else
    #define DISPLAY_LOG(...)
#endif

#if DEBUG_PWM
    #define PWM_LOG(fmt, ...) Serial.printf("[PWM] " fmt, ##__VA_ARGS__)
#else
    #define PWM_LOG(...)
#endif