/*************************************************************
  ESP32 + ERa + INA219 + Servo + LDR (Dual-axis Solar Tracker)
*************************************************************/

#define ERA_DEBUG
#define DEFAULT_MQTT_HOST "mqtt1.eoh.io"
#define ERA_AUTH_TOKEN "1a7f9a1a-8f90-4afa-b97f-5597e2c38c17"

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <ERa.hpp>
#include <ESP32Servo.h>

// WiFi
const char ssid[] = "P708";
const char pass[] = "12345678910";

// INA219
Adafruit_INA219 ina219;
bool ina219_available = false;

// ERa
WiFiClient mbTcpClient;

// LDR pins (ADC1)
int ldrlt = 34; // Top Left
int ldrrt = 35; // Top Right
int ldrld = 32; // Bottom Left
int ldrrd = 33; // Bottom Right

// Servo setup
Servo horizontal;
Servo vertical;
int servohori = 90;  // Initial position
int servovert = 45;  // Initial position

const int servohoriPin = 12;
const int servovertPin = 13;
const int servohoriLimitHigh = 175;
const int servohoriLimitLow  = 5;
const int servovertLimitHigh = 100;
const int servovertLimitLow  = 1;
const int tolerance = 90;

/* ===== ERa Connection Events ===== */
ERA_CONNECTED() {
    ERA_LOG(ERA_PSTR("ERa"), ERA_PSTR("ERa connected!"));
}

ERA_DISCONNECTED() {
    ERA_LOG(ERA_PSTR("ERa"), ERA_PSTR("ERa disconnected!"));
}

/* ===== INA219 Reporting + LDR Tracking ===== */
void timerEvent() {
    // LDR đọc giá trị
    int lt = analogRead(ldrlt);
    int rt = analogRead(ldrrt);
    int ld = analogRead(ldrld);
    int rd = analogRead(ldrrd);

    int avt = (lt + rt) / 2;
    int avd = (ld + rd) / 2;
    int avl = (lt + ld) / 2;
    int avr = (rt + rd) / 2;

    int dvert = avt - avd;
    int dhoriz = avl - avr;

    // Vertical tracking
    if (abs(dvert) > tolerance) {
        if (avt > avd) {
            servovert++;
            if (servovert > servovertLimitHigh) servovert = servovertLimitHigh;
        } else {
            servovert--;
            if (servovert < servovertLimitLow) servovert = servovertLimitLow;
        }
        vertical.write(servovert);
    }

    // Horizontal tracking
    if (abs(dhoriz) > tolerance) {
        if (avl > avr) {
            servohori--;
            if (servohori < servohoriLimitLow) servohori = servohoriLimitLow;
        } else {
            servohori++;
            if (servohori > servohoriLimitHigh) servohori = servohoriLimitHigh;
        }
        horizontal.write(servohori);
    }

    // INA219 readings
    if (ina219_available) {
        float voltage = ina219.getBusVoltage_V();     // V
        float current = ina219.getCurrent_mA();       // mA
        float power   = voltage * (current / 1000.0);  // W

        // Gửi dữ liệu lên ERa (power * 1000 để gửi theo đơn vị mW nếu muốn)
        ERa.virtualWrite(V0, voltage);
        ERa.virtualWrite(V1, current);
        ERa.virtualWrite(V2, power * 1000.0);  // mW

        // In ra Serial
        ERA_LOG(ERA_PSTR("INA219"), ERA_PSTR("V: %.2f V | I: %.2f mA | P: %.2f mW"),
                voltage, current, power * 1000.0);
    }
}

void setup() {
#if defined(ERA_DEBUG)
    Serial.begin(115200);
#endif

    // INA219
    if (!ina219.begin()) {
        Serial.println("Không tìm thấy INA219.");
        ina219_available = false;
    } else {
        ina219_available = true;
    }

    // Servo setup
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    horizontal.setPeriodHertz(50);
    horizontal.attach(servohoriPin, 500, 2500);
    vertical.setPeriodHertz(50);
    vertical.attach(servovertPin, 500, 2500);

    horizontal.write(servohori);
    vertical.write(servovert);

    // ERa setup
    ERa.setModbusClient(mbTcpClient);
    ERa.setScanWiFi(true);
    ERa.begin(ssid, pass);
    ERa.addInterval(1000L, timerEvent);  // 1 giây gọi lại
}

void loop() {
    ERa.run();
}
