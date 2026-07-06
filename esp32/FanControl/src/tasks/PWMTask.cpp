#include <Arduino.h>
#include <cstdint>
#include "esp32-hal-ledc.h"
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

uint8_t exhaustPWM(uint8_t duty) {
    constexpr uint8_t offset = (10 * 255) / 100; // ≈25
    return (duty > offset) ? duty - offset : 0;
}

void taskPWM(void *pvParameters){
    PWM_LOG("Started...\n");

    Packet packet;

    const TickType_t timeout = pdMS_TO_TICKS(2000);

    const uint8_t pwmPins[] = {
        Pins::PWM_PIN_0,
        Pins::PWM_PIN_1,
        Pins::PWM_PIN_2,
        Pins::PWM_PIN_3
    };

    for (uint8_t i = 0; i < 4; i++) {
    ledcSetup(i, Constants::FREQUENCY, Constants::RESOLUTION);
    ledcAttachPin(pwmPins[i], i);
    }   

    
    while (true){
        if (xQueueReceive(queuePWM, &packet, timeout)){
            uint8_t pwm = percentToPWM(packet.rpm);

            if (pwm < 1){
                pwm = 255;
            }

            uint8_t pwm_exhaust = exhaustPWM(pwm);

            ledcWrite(0, pwm);
            ledcWrite(1, pwm);
            ledcWrite(2, pwm_exhaust);
            ledcWrite(3, pwm_exhaust);

            PWM_LOG("Duty intake: %d | Duty exhaust: %d | Send to GPIOs: %d, %d, %d, %d.",
                    pwm,
                    pwm_exhaust,
                    pwmPins[0],
                    pwmPins[1],
                    pwmPins[2],
                    pwmPins[3]);
        } else {
            ledcWrite(0, 127);
            ledcWrite(1, 127);
            ledcWrite(2, 102);
            ledcWrite(3, 102);
        }

        PWM_LOG("Free memory: %d\n", uxTaskGetStackHighWaterMark(NULL));
        vTaskDelay(1);
    }
}

void startTaskPWM(){
    xTaskCreatePinnedToCore(
        taskPWM,
        "PWM duty to pwm pin",
        4096,
        NULL,
        2,
        NULL,
        1
    );
}