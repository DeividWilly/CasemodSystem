#include <Arduino.h>
#include "SerialTask.h"
#include "../serial/SerialProtocol.h"
#include "../serial/QueueManager.h"

void taskSerial(void *pvParameters){

    while (true)
{
    Packet packet;

    //Serial.printf("TX temp=%d load=%d rpm=%d \n", packet.temp, packet.load, packet.rpm);

    if (readPacket(packet)){
        Serial.println("PACKET OK");
        Serial.printf("queueDisplay Serial=%p\n", queueDisplay);
        xQueueOverwrite(queuePWM, &packet);
        xQueueOverwrite(queueDisplay, &packet);
    } else {
        //Serial.println("PACKET FAIL");
    }
    //Serial.printf("SerialTask: Memória sobrando: %d\n", uxTaskGetStackHighWaterMark(NULL));
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