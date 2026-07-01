#include <Arduino.h>
#include "SensorsTask.h"
#include "../sensors/sensors.h"
#include "../sensors/QueueManager.h"
#include "../sensors/INA226.h"
#include "../sensors/DS18B20.h"

void taskSensors(void *pvParameters) {
    SensorsData data;

    while (true) {
        data.fontTemperature = readTemperature();

        data.voltage12 = readVoltage12();
        data.current12 = readCurrent12();

        data.voltage19 = readVoltage19();
        data.current19 = readCurrent19();

        Serial.printf("T=%.1f V12=%.1f V19=%.1f I19=%.1f\n", data.fontTemperature, data.voltage12, data.voltage19, data.current19);

        xQueueOverwrite(queueSensors, &data);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void startTaskSensors(){
    xTaskCreatePinnedToCore(
        taskSensors,
        "Read sensors",
        2048,
        NULL,
        4,
        NULL,
        1
    );
}