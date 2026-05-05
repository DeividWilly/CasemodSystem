#include "freertos/portmacro.h"
#include <Arduino.h>

const uint8_t PWM_PIN = 19;
const uint8_t PWM_CHANNEL = 0;

const uint16_t FREQUENCY = 25000;
const uint8_t RESOLUTION = 8;

uint8_t percentToPWM(uint8_t percent) {
    if (percent > 100) percent = 100;
    return (uint8_t)((percent / 100.0) * 255);
}

struct __attribute__((packed)) Packet {
    uint8_t temp;
    uint8_t load;
    uint8_t rpm;
};
volatile Packet packet;

void taskSerial(void *pvParameters){
    while (true){

        if (Serial.available() >= sizeof(Packet)) {
            Serial.println("Recebendo serial");

            Serial.readBytes((uint8_t*)&packet, sizeof(Packet));

            uint8_t pwm = percentToPWM(packet.rpm);

            Serial.printf("Temp: %d | Load: %d | RPM: %d | DUTY: %d\n",
                packet.temp,
                packet.load,
                packet.rpm,
                pwm
            );

            Serial.println(uxTaskGetStackHighWaterMark(NULL));
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void setup() {
    Serial.begin(115200);

    delay(1000);

    ledcSetup(PWM_CHANNEL, FREQUENCY, RESOLUTION);
    ledcAttachPin(PWM_PIN, PWM_CHANNEL);

    xTaskCreatePinnedToCore(
        taskSerial,
        "RX Date",
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