#include "DisplayTask.h"
#include <Arduino.h>
#include "../serial/QueueManager.h"
#include "../serial/Packet.h"
#include "../display/Update.h"
#include "../config/Debug.h"
#include "../display/DisplayDriver.h"
#include "../sensors/QueueManager.h"
#include "../sensors/sensors.h"

void taskDisplay(void *pvParameters) {
    DISPLAY_LOG("Started...\n");
    Packet packet;
    SensorsData sensors;

    Packet lastPacket = {255, 255, 255, 65535, 65535};

    Serial.printf("queueDisplay=%p\n", queueDisplay);
    Serial.printf("queueSensors=%p\n", queueSensors);

    const TickType_t timeout = pdMS_TO_TICKS(2000);

    while (true) {
        //xQueueReceive(queueSensors, &sensors, 0);
        if (xQueueReceive(queueSensors, &sensors, 0) == pdTRUE) {
            Serial.printf("Display recebeu V12=%.1f\n", sensors.voltage12);
        } else {
            Serial.println("Sem dados na queueSensors");
        }
        DISPLAY_LOG("Waiting packet... \n");


        if (xQueueReceive(queueDisplay, &packet, timeout)) {

            DISPLAY_LOG("RX temp=%d load=%d rpm=%d uram=%u tram=%u\n", packet.temp, packet.load, packet.rpm, packet.uram, packet.tram);

            bool changed =
                packet.temp != lastPacket.temp ||
                packet.load != lastPacket.load ||
                packet.rpm  != lastPacket.rpm  ||
                packet.uram != lastPacket.uram ||
                packet.tram != lastPacket.tram;

            DISPLAY_LOG("changed=%d\n", changed);
            DISPLAY_LOG("last=%d current=%d\n", lastPacket.temp, packet.temp);

            if (changed) {
                updateHeader(display,
                            packet.temp,
                            packet.load,
                            packet.rpm,
                            packet.uram,
                            packet.tram,
                            sensors.fontTemperature);
                lastPacket = packet;
            }
        }

        DISPLAY_LOG("Free memory: %d\n",uxTaskGetStackHighWaterMark(NULL));
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void startDisplayTask(){
    Serial.println("Criando display...");
    xTaskCreatePinnedToCore(
        taskDisplay,
        "Show info in display epaper",
        3074,
        NULL,
        1,
        NULL,
        1
    );
}