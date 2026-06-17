#include "DisplayTask.h"
#include <Arduino.h>
#include "../serial/QueueManager.h"
#include "../serial/Packet.h"
#include "../display/DisplayDriver.h"
#include "../display/Update.h"
#include "../display/Layout.h"

void taskDisplay(void *pvParameters) {
    Serial.println("DisplayTask iniciado...");
    Packet packet;

    Packet lastPacket = {255, 255, 255, 65535, 65535};

    Serial.printf("queueDisplay=%p\n", queueDisplay);

    const TickType_t timeout = pdMS_TO_TICKS(2000);

    while (true) {
        Serial.println("Esperando pacote...");
        if (xQueueReceive(queueDisplay, &packet, timeout)) {
            Serial.println("RECEIVED");
            //Serial.printf(
            //    "RX temp=%d load=%d rpm=%d\n",
            //    packet.temp,
            //    packet.load,
            //    packet.rpm
            //);
            bool changed =
                packet.temp != lastPacket.temp ||
                packet.load != lastPacket.load ||
                packet.rpm  != lastPacket.rpm  ||
                packet.uram != lastPacket.uram ||
                packet.tram != lastPacket.tram;
            //Serial.printf("changed=%d\n", changed);
            //Serial.printf("last=%d current=%d\n",lastPacket.temp, packet.temp);
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

        Serial.printf("displayTask: Memória sobrando: %d\n", uxTaskGetStackHighWaterMark(NULL));
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