#include "esp32-hal-gpio.h"
#include "esp32-hal-ledc.h"
#include "freertos/portmacro.h"
#include <Arduino.h>
#include <cstdint>
#include <SPI.h>
#include <GxEPD2_BW.h>
// #include <Fonts/FreeMonoBold12pt7b.h>
#include "display/Layout.h"
#include "display/Update.h"
#include "display/DisplayContext.h"
#include "Inconsolata_Bold12pt7b.h"

uint8_t pwm = 127;

const uint8_t PWM_PIN_0 = 19;
//const uint8_t PWM_PIN_1 = x;
//const uint8_t PWM_PIN_2 = x;
const uint8_t PWM_CHANNEL = 0;

const uint16_t FREQUENCY = 1000;
const uint8_t RESOLUTION = 8;

const uint8_t EPD_MOSI = 23;
const uint8_t EPD_SCK = 18;
const uint8_t EPD_CS = 5;
const uint8_t EPD_DC = 17;
const uint8_t EPD_RES = 16;
const uint8_t EPD_BUSY = 25;

GxEPD2_BW<GxEPD2_370_GDEY037T03,
           GxEPD2_370_GDEY037T03::HEIGHT>
display(
    GxEPD2_370_GDEY037T03(
        EPD_CS,
        EPD_DC,
        EPD_RES,
        EPD_BUSY
    )
);

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

SemaphoreHandle_t displayMutex;
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

    const TickType_t timeout = pdMS_TO_TICKS(2000);
    while (true){

        if (xQueueReceive(queuePWM, &packet, timeout)){
            uint8_t pwm = percentToPWM(packet.rpm);

            if (pwm < 1){
                pwm = 255;
            }

            ledcWrite(PWM_CHANNEL, pwm);
            Serial.printf("DUTY: %d\n", pwm);
            // Serial.printf("Duty de %d enviado para o pino %d!\n", pwm, PWM_PIN);
        } else {
            ledcWrite(PWM_CHANNEL, 127);
        }
        //Serial.printf("Memória sobrando: %d\n", uxTaskGetStackHighWaterMark(NULL));
        vTaskDelay(1);
        
    }
}

void taskDisplay(void *pvParameters)
{
    Packet packet;

    Packet lastPacket = {255, 255, 255, 65535, 65535};

    const TickType_t timeout = pdMS_TO_TICKS(2000);

    while (true)
    {
        if (xQueueReceive(queueDisplay, &packet, timeout))
        {
            bool changed =
                packet.temp != lastPacket.temp ||
                packet.load != lastPacket.load ||
                packet.rpm  != lastPacket.rpm  ||
                packet.uram != lastPacket.uram ||
                packet.tram != lastPacket.tram;

            if (changed)
            {
                updateHeader(
                    display,
                    packet.temp,
                    packet.load,
                    packet.rpm,
                    packet.uram,
                    packet.tram
                );

                lastPacket = packet;
            }
        }
        Serial.printf("Memória sobrando: %d\n", uxTaskGetStackHighWaterMark(NULL));
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void setup() {
    displayMutex = xSemaphoreCreateMutex();

    Serial.begin(115200);
    SPI.begin(EPD_SCK, -1, EPD_MOSI, EPD_CS);

    pinMode(EPD_BUSY, INPUT);

    display.init(115200);
    display.setRotation(1);
    display.setFont(&Inconsolata_Bold12pt7b);
    display.setTextColor(GxEPD_BLACK);
    display.setFullWindow();
    
    drawStaticLayout(display);

    queuePWM = xQueueCreate(1, sizeof(Packet));
    queueDisplay = xQueueCreate(1, sizeof(Packet));

    if (!queuePWM || !queueDisplay) {
        Serial.println("ERRO QUEUE");
        while (true);
    }

    ledcSetup(PWM_CHANNEL, FREQUENCY, RESOLUTION);
    ledcAttachPin(PWM_PIN_0, PWM_CHANNEL);

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
        4096,
        NULL,
        1,
        NULL,
        1
    );

    Serial.println("ESP32 pronto!");
}

void loop() {
}