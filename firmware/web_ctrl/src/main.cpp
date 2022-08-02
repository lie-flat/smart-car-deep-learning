#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <constants.h>

// Be sure to define the WiFi mode before include lie-flat.h
#define LIE_FLAT_WIFI_STA
#include <lie-flat.h>

AsyncWebServer server(80);
Adafruit_MPU6050 mpu;

void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

inline __attribute__((always_inline)) float parse_param(
    AsyncWebServerRequest* request,
    const char* param) {
  return request->getParam(param, true)->value().toFloat();
}

const int optocoupler = 14;
volatile int optocoupler_state = 0;

void optocoupler_interrupt() {
  // Serial.println("INFO: Optocoupler interrupt!");
  // Serial.printf("INFO: Analog read: %d\n", analogRead(optocoupler));
  optocoupler_state += digitalRead(optocoupler);
}

void setup() {
  // put your setup code here, to run once:
  // Motor & Servo
  init_motor(motorA, motorB);
  init_servo(servo);
  set_servo(7.5);
  servo_start_pwm();
  // Serial
  init_serial();
  // Connect to WiFi / Start an AP
  init_wifi();
  // MPU6050
  init_mpu(mpu, mpuSDA, mpuSCL);
  // Optocoupler
  attachInterrupt(digitalPinToInterrupt(optocoupler), optocoupler_interrupt,
                  FALLING);
  // Web server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/plain", "Hello, world");
  });
  server.on("/cmd", HTTP_POST, [](AsyncWebServerRequest* request) {
    if (request->hasParam(SERVO_PARAM, true))
      set_servo(parse_param(request, SERVO_PARAM));
    if (request->hasParam(MOTOR_A_PARAM, true))
      set_a(parse_param(request, MOTOR_A_PARAM));
    if (request->hasParam(MOTOR_B_PARAM, true))
      set_b(parse_param(request, MOTOR_B_PARAM));
    request->send(200, "text/plain", "OK");
  });
  server.on("/ping", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/plain", "Pong");
  });
  server.on("/read", HTTP_GET, [](AsyncWebServerRequest* request) {
    sensors_event_t acc, gyro, temp;
    // acc: m/s^2
    // gyro: rad/s
    // temp: Celsius
    mpu.getEvent(&acc, &gyro, &temp);
    request->send(200, "application/json",
                  "{\"acceleration\":{"
                  "\"x\":" +
                      String(acc.acceleration.x) +
                      ",\"y\":" + String(acc.acceleration.y) +
                      ",\"z\":" + String(acc.acceleration.z) +
                      "},\"gyro\":{"
                      "\"x\":" +
                      String(gyro.gyro.x) + ",\"y\":" + String(gyro.gyro.y) +
                      ",\"z\":" + String(gyro.gyro.z) +
                      "},\"temp\":" + String(temp.temperature) + "}");
  });
  server.on("/optocoupler", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/plain", String(optocoupler_state));
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}