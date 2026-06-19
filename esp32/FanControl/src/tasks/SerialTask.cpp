#include <Arduino.h>
#include "SerialTask.h"
#include "../serial/SerialProtocol.h"
#include "../serial/QueueManager.h"
#include "../config/Debug.h"

void taskSerial(void *pvParameters){
    SERIAL_LOG("Started...\n");

    while (true)
{
    Packet packet;

    SERIAL_LOG("TX temp=%d load=%d rpm=%d uram=%d tram=%d\n", packet.temp, packet.load, packet.rpm, packet.uram, packet.tram);
    if (readPacket(packet)){

        SERIAL_LOG("PACKET OK\n");
        SERIAL_LOG("queueDisplay Serial=%p\n", queueDisplay);

        xQueueOverwrite(queuePWM, &packet);
        xQueueOverwrite(queueDisplay, &packet);
    }
    SERIAL_LOG("Memory free: %d\n", uxTaskGetStackHighWaterMark(NULL));
    vTaskDelay(1);
}
}

void startTaskSerial(){
    xTaskCreatePinnedToCore(
        taskSerial,
        "Receive data from PC via serial",
        3072,
        NULL,
        3,
        NULL,
        0
    );
}