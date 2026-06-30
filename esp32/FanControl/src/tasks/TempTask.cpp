#include "freertos/portmacro.h"
#include <Arduino.h>
#include "../sensors/DS18B20.h"
#include "../sensors/QueueManager.h"

void taskTemp(void *pvParameters){
    while (true) {
        float temperature = readTemperature();

        xQueueSend(queueDisplayTemp, &temperature, portMAX_DELAY);

        vTaskDelay(1000);
    }
}

void startTaskTemp(){
    xTaskCreatePinnedToCore(
        taskTemp,
        "Read temperature",
        2048,
        NULL,
        4,
        NULL,
        1
    );
}