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

    displayInit();

    queuePWM = xQueueCreate(5, sizeof(Packet));
    queueDisplay = xQueueCreate(5, sizeof(Packet));

    startDisplayTask();
    
    startTaskPWM();

    startTaskSerial();

}

void loop(){}