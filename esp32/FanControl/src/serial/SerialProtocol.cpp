#include "SerialProtocol.h"
#include "esp32-hal.h"

#include <Arduino.h>

bool readPacket(Packet& packet)
{
    while (Serial.available())
    {
        uint8_t byte = Serial.read();

        if (byte == 0xAA){
            uint32_t start = millis();

            while (Serial.available() < sizeof(Packet)){
                if(millis() - start > 100)
                    return false;

                vTaskDelay(1);
            }

            return Serial.readBytes(
                (uint8_t*)&packet,
                sizeof(Packet)
            ) == sizeof(Packet);
        }
    }

    return false;
}