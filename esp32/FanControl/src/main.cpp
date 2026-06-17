#include <Arduino.h>

#include "tasks/DisplayTask.h"
#include "tasks/PWMTask.h"
#include "tasks/SerialTask.h"
#include "display/DisplayDriver.h"
#include "serial/QueueManager.h"
#include "serial/Packet.h"

void setup()
{
    Serial.begin(115200);
    Serial.println("Serial iniciado.");

    displayInit();
    Serial.println("Display iniciado.");

    queuePWM = xQueueCreate(1, sizeof(Packet));
    queueDisplay = xQueueCreate(1, sizeof(Packet));

    startDisplayTask();
    Serial.println("Tarefa display iniciada");
    
    startTaskPWM();
    Serial.println("Tarefa PWM iniciada");

    startTaskSerial();
    Serial.println("Tarefa serial iniciada");

    Serial.printf("Packet size=%u\n", sizeof(Packet));
}

void loop(){}