#include <Arduino.h>

uint8_t percentToPWM(uint8_t percent) {
    if (percent > 100) percent = 100;
    return (uint8_t)((percent / 100.0) * 255);
}

const uint8_t PWM_PIN = 19;
const uint8_t PWM_CHANNEL = 0;

const uint16_t FREQUENCY = 25000;
const uint8_t RESOLUTION = 8;

struct __attribute__((packed)) Packet {
    uint8_t temp;
    uint8_t load;
    uint8_t rpm;
};

Packet packet;

void setup() {
    Serial.begin(115200);
    delay(1000);

    ledcSetup(PWM_CHANNEL, FREQUENCY, RESOLUTION);
    ledcAttachPin(PWM_PIN, PWM_CHANNEL);

    Serial.println("ESP32 pronto!");
}

void loop() {

    // 🔹 Espera receber os 3 bytes
    if (Serial.available() >= sizeof(Packet)) {

        Serial.readBytes((char*)&packet, sizeof(Packet));

        uint8_t pwm = percentToPWM(packet.rpm);

        ledcWrite(PWM_CHANNEL, pwm);

        Serial.printf(
            "Temp: %d | Load: %d | RPM: %d | PWM: %d\n",
            packet.temp,
            packet.load,
            packet.rpm,
            pwm
        );
      
    }
    delay(500);
}