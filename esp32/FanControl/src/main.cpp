#include <Arduino.h>

#include "sensors/DS18B20.h"
#include "sensors/sensors.h"
#include "tasks/DisplayTask.h"
#include "tasks/PWMTask.h"
#include "tasks/SensorsTask.h"
#include "tasks/SerialTask.h"
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
    queueSensors = xQueueCreate(1, sizeof(SensorsData));

    //ds18b20begin();

    startDisplayTask();
    
    startTaskPWM();

    startTaskSerial();

    startTaskSensors();

    Serial.printf("Packet size=%u\n", sizeof(Packet));
}

void loop(){}