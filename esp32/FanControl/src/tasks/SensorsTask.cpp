#include <Arduino.h>
#include "SensorsTask.h"
#include "../sensors/sensors.h"
#include "../sensors/QueueManager.h"
#include "../sensors/INA226.h"
#include "../sensors/DS18B20.h"
#include "../config/Debug.h"

void taskSensors(void *pvParameters) {
    SENSORS_LOG("Started\n");
    SensorsData data;

    while (true) {
        data.fontTemperature = readTemperature();

        data.voltage12 = readVoltage12();
        data.current12 = readCurrent12();
        data.power12 = readPower12();

        data.voltage19 = readVoltage19();
        data.current19 = readCurrent19();
        data.power19 = readPower19();


        SENSORS_LOG("TX fontTemperature=%.0f voltage12=%.1f current12=%.1f power12=%.1f voltage19=%.1f current19=%.1f power19=%.1f\n",
                        data.fontTemperature,
                        data.voltage12,
                        data.current12,
                        data.power12,
                        data.voltage19,
                        data.current19,
                        data.power19);

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