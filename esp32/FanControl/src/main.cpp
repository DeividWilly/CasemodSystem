#include <Arduino.h>

#include "tasks/DisplayTask.h"
#include "tasks/PWMTask.h"
#include "tasks/SerialTask.h"

void setup()
{
    Serial.begin(115200);

    startDisplayTask();
    
    startTaskPWM();

    startTaskSerial();

}

void loop(){}