#include "DisplayTask.h"
#include <Arduino.h>
#include "../serial/QueueManager.h"
#include "../serial/Packet.h"
#include "../display/Update.h"
#include "../config/Debug.h"
#include "../sensors/QueueManager.h"
#include "../sensors/sensors.h"
#include "../display/DisplayDriver.h"

void taskDisplay(void *pvParameters) {
    DISPLAY_LOG("Started...\n");
    Packet packet;
    SensorsData sensors;

    Packet lastPacket = {255, 255, 255, 65535, 65535};

    Serial.printf("queueDisplay=%p\n", queueDisplay);
    Serial.printf("queueSensors=%p\n", queueSensors);

    const TickType_t timeout = pdMS_TO_TICKS(2000);

    while (true) {
        if (xQueueReceive(queueSensors, &sensors, 0) == pdTRUE) {
            SENSORS_LOG("RX fontTemperature=%.0f voltage12=%.1f current12=%.1f power12=%.1f voltage19=%.1f current19=%.1f power19=%.1f\n",
                        sensors.fontTemperature,
                        sensors.voltage12,
                        sensors.current12,
                        sensors.power12,
                        sensors.voltage19,
                        sensors.current19,
                        sensors.power19);
        } else {
            SENSORS_LOG("NO DATA in queueSensors");
        }
        
        DISPLAY_LOG("Waiting packet... \n");

        if (xQueueReceive(queueDisplay, &packet, timeout)) {

            DISPLAY_LOG("RX temp=%d load=%d rpm=%d uram=%u tram=%u\n", 
                        packet.temp, 
                        packet.load, 
                        packet.rpm, 
                        packet.uram, 
                        packet.tram);

            bool changed =
                packet.temp != lastPacket.temp ||
                packet.load != lastPacket.load ||
                packet.rpm  != lastPacket.rpm  ||
                packet.uram != lastPacket.uram ||
                packet.tram != lastPacket.tram;

            DISPLAY_LOG("changed=%d\n", changed);
            DISPLAY_LOG("last=%d current=%d\n", lastPacket.temp, packet.temp);

            if (changed) {
                updateDisplay(display,
                            packet.temp,
                            packet.load,
                            packet.rpm,
                            packet.uram,
                            packet.tram,
                            sensors.fontTemperature,
                            sensors.voltage12,
                            sensors.current12,
                            sensors.power12,
                            sensors.voltage19,
                            sensors.current19,
                            sensors.power19);
                lastPacket = packet;
            }
        }

        DISPLAY_LOG("Free memory: %d\n",uxTaskGetStackHighWaterMark(NULL));
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void startDisplayTask(){
    Serial.println("Starting display...");
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