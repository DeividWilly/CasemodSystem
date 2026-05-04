#include <Arduino.h>

uint8_t percentToPWM(uint8_t percent) {

    if (percent > 100)
        percent = 100;

    return (uint8_t)((percent / 100.0) * 255);
}

const uint8_t PWM_PIN = 19;

const uint8_t CS_PIN   = 5;
const uint8_t DC_PIN   = 17;
const uint8_t RES_PIN  = 16;
const uint8_t BUSY_PIN = 25;

const uint16_t FREQUENCY = 25000;
const uint8_t RESOLUTION = 8;

volatile uint8_t temp;
volatile uint8_t rpm;
// const uint8_t load;
// const uint8_t uram;
// const uint8_t tram;

void setup() {

    Serial.begin(115200);

    Serial.println("Hello, ESP32!");

    ledcAttach(PWM_PIN, FREQUENCY, RESOLUTION);
}

void loop() {

    uint8_t rpm = 0;

    uint8_t pwm = percentToPWM(rpm);

    ledcWrite(PWM_PIN, pwm);

    Serial.printf("PWM definido para %d!\n", pwm);

    delay(500);
}
