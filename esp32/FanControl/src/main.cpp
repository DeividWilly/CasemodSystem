#include "esp32-hal-ledc.h"
#include "freertos/portmacro.h"
#include <Arduino.h>
#include <cstdint>

const uint8_t PWM_PIN = 19;
const uint8_t PWM_CHANNEL = 0;

const uint16_t FREQUENCY = 25000;
const uint8_t RESOLUTION = 8;

uint8_t percentToPWM(uint8_t percent) {
    if (percent > 100) percent = 100;
    return (uint8_t)((percent / 100.0) * 255);
}

struct __attribute__((packed)) Packet {
    uint8_t temp; // -> display
    uint8_t load; // -> display
    uint8_t rpm; // -> Pwm Control
};

QueueHandle_t queuePWM;
QueueHandle_t queueDisplay;

uint8_t pwm = 255;

SemaphoreHandle_t mutex;

void taskSerial(void *pvParameters){
    Packet packet;
    while (true){
        if (Serial.available() >= sizeof(Packet)) {
            Serial.println("Recebendo serial");

            Serial.readBytes((uint8_t*)&packet, sizeof(Packet));

            xQueueOverwrite(queuePWM, &packet);
            xQueueOverwrite(queueDisplay, &packet);

            Serial.printf("Temp: %d | Load: %d | RPM: %d\n",
                packet.temp,
                packet.load,
                packet.rpm
            );

            Serial.println(uxTaskGetStackHighWaterMark(NULL));
        }

        vTaskDelay(1);
    }
}

void taskPWM(void *pvParameters){
    Packet packet;
    while (true){

        if (xQueueReceive(queuePWM, &packet, portMAX_DELAY)){
            uint8_t pwm = percentToPWM(packet.rpm);

            if (pwm < 1){
                pwm = 255;
            }

            ledcWrite(PWM_CHANNEL, pwm);
            Serial.printf("Duty de %d enviado para o pino %d!\n", pwm, PWM_PIN);
        }
        Serial.println(uxTaskGetStackHighWaterMark(NULL));
        vTaskDelay(1);
        
    }
}

void setup() {
    Serial.begin(115200);

    delay(1000);

    queuePWM = xQueueCreate(1, sizeof(Packet));
    queueDisplay = xQueueCreate(1, sizeof(Packet));

    if (!queuePWM || !queueDisplay) {
        Serial.println("ERRO QUEUE");
        while (true);
    }

    ledcSetup(PWM_CHANNEL, FREQUENCY, RESOLUTION);
    ledcAttachPin(PWM_PIN, PWM_CHANNEL);

    xTaskCreatePinnedToCore(
        taskSerial,
        "Receive data from PC",
        2048,
        NULL,
        3,
        NULL,
        0
    );

    xTaskCreatePinnedToCore(
        taskPWM,
        "PWM duty to Pin",
        2048,
        NULL,
        2,
        NULL,
        0
    );

    Serial.println("ESP32 pronto!");
}

void loop() {
}