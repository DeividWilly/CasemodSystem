#include <Arduino.h>
#include "SerialTask.h"
#include "../serial/SerialProtocol.h"

void taskSerial(void *pvParameters){
    Packet packet;

    while (true)
{
    Packet packet;

    if (readPacket(packet)){
        xQueueOverwrite(queuePWM, &packet);
        xQueueOverwrite(queueDisplay, &packet);
    }

    vTaskDelay(1);
}
}

void startTaskSerial(){
    xTaskCreatePinnedToCore(
        taskSerial,
        "Receive data from PC via serial",
        1024,
        NULL,
        3,
        NULL,
        0
    );
}