#include "DisplayTask.h"
#include <Arduino.h>
#include "../serial/QueueManager.h"
#include "../serial/Packet.h"
#include "../display/DisplayDriver.h"
#include "../display/Update.h"
#include "../display/Layout.h"

void taskDisplay(void *pvParameters) {
    Packet packet;

    Packet lastPacket = {255, 255, 255, 65535, 65535};

    const TickType_t timeout = pdMS_TO_TICKS(2000);

    while (true) {
        if (xQueueReceive(queueDisplay, &packet, timeout)) {
            bool changed =
                packet.temp != lastPacket.temp ||
                packet.load != lastPacket.load ||
                packet.rpm  != lastPacket.rpm  ||
                packet.uram != lastPacket.uram ||
                packet.tram != lastPacket.tram;

            if (changed) {
                updateHeader(display,
                            packet.temp,
                            packet.load,
                            packet.rpm,
                            packet.uram,
                            packet.tram);
                
                lastPacket = packet;
            }
        }

        Serial.printf("Memória sobrando: %d\n", uxTaskGetStackHighWaterMark(NULL));
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void startDisplayTask(){
    xTaskCreatePinnedToCore(
        taskDisplay,
        "Show info in display epaper",
        4096,
        NULL,
        1,
        NULL,
        1
    );
}