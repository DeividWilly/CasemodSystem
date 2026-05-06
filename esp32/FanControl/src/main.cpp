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
    uint16_t uram; // -> display
    uint16_t tram; // -> display
};

QueueHandle_t queuePWM;
QueueHandle_t queueDisplay;


void taskSerial(void *pvParameters){
    Packet packet;

    while (true){

        while (Serial.available()) {

            uint8_t byte = Serial.read();

            if (byte == 0xAA) {
                while (Serial.available() < sizeof(Packet)) {
                    vTaskDelay(1);
                }

                if (Serial.readBytes((uint8_t*)&packet, sizeof(Packet)) == sizeof(Packet)) {

                    xQueueOverwrite(queuePWM, &packet);
                    xQueueOverwrite(queueDisplay, &packet);
                }
            }
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
            Serial.printf("DUTY: %d\n", pwm);
            // Serial.printf("Duty de %d enviado para o pino %d!\n", pwm, PWM_PIN);
        }
        //Serial.printf("Memória sobrando: %d\n", uxTaskGetStackHighWaterMark(NULL));
        vTaskDelay(1);
        
    }
}

void taskDisplay(void *pvParameters){
    Packet packet;
    while (true){
        if (xQueueReceive(queueDisplay, &packet, portMAX_DELAY)){
            float tram = packet.tram / 10.0;
            float uram = packet.uram / 10.0;
            Serial.printf("DISPLAY: Temp: %d | Load: %d | RAM: %g/%gGB\n", packet.temp, packet.load, uram, tram);
            Serial.printf("Memória sobrando: %d\n", uxTaskGetStackHighWaterMark(NULL));

        }
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
        1024,
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
        1
    );

    xTaskCreatePinnedToCore(
        taskDisplay,
        "Show stats on display",
        8192,
        NULL,
        1,
        NULL,
        1
    );

    Serial.println("ESP32 pronto!");
}

void loop() {
}