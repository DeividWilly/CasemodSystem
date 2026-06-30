#include <Arduino.h>

#include "tasks/DisplayTask.h"
#include "tasks/PWMTask.h"
#include "tasks/SerialTask.h"
#include "tasks/TempTask.h"
#include "display/DisplayDriver.h"
#include "serial/QueueManager.h"
#include "serial/Packet.h"
#include "sensors/QueueManager.h"

void setup()
{
    Serial.begin(115200);
    Serial.println("Serial init.");

    displayInit(); 

    queuePWM = xQueueCreate(1, sizeof(Packet));
    queueDisplay = xQueueCreate(1, sizeof(Packet));
    queueDisplayTemp = xQueueCreate(5, sizeof(float));

    startDisplayTask();
    
    startTaskPWM();

    startTaskSerial();

    startTaskTemp();

    Serial.printf("Packet size=%u\n", sizeof(Packet));
}

void loop(){}