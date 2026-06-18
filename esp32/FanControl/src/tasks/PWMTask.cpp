#include <Arduino.h>
#include "freertos/projdefs.h"
#include "PWMTask.h"
#include "config/Constants.h"
#include "config/Pins.h"
#include "../serial/Packet.h"
#include "../serial/QueueManager.h"
#include "config/Debug.h"

uint8_t percentToPWM(uint8_t percent) {
        if (percent > 100) percent = 100;
        return (uint8_t)((percent / 100.0) * 255);
    }

void taskPWM(void *pvParameters){
    PWM_LOG("Started...\n");

    Packet packet;

    const TickType_t timeout = pdMS_TO_TICKS(2000);
    ledcSetup(Constants::PWM_CHANNEL, Constants::FREQUENCY, Constants::RESOLUTION);
    ledcAttachPin(Pins::PWM_PIN_0, Constants::PWM_CHANNEL);
    while (true){
        if (xQueueReceive(queuePWM, &packet, timeout)){
            uint8_t pwm = percentToPWM(packet.rpm);

            if (pwm < 1){
                pwm = 255;
            }

            ledcWrite(Constants::PWM_CHANNEL, pwm);
            PWM_LOG("Duty: %d\n", pwm);

            // Serial.printf("Duty de %d enviado para o pino %d!\n", pwm, PWM_PIN);
            PWM_LOG("Duty sended to GPIO %d.\n", Pins::PWM_PIN_0);
        } else {
            ledcWrite(Constants::PWM_CHANNEL, 127);
        }
        
        PWM_LOG("Free memory: %d\n", uxTaskGetStackHighWaterMark(NULL));
        vTaskDelay(1);
    }
}

void startTaskPWM(){
    xTaskCreatePinnedToCore(
        taskPWM,
        "PWM duty to pwm pin",
        2048,
        NULL,
        2,
        NULL,
        1
    );
}