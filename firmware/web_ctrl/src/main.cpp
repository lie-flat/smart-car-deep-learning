#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <config.h>
#include <constants.h>
#include <lie-flat.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

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

void setup() {
  // put your setup code here, to run once:
  // Motor & Servo
  init_motor(motorA, motorB);
  init_servo(servo);
  set_servo(7.5);
  servo_start_pwm();
  // Serial
  Serial.begin(115200);
  while (!Serial) {
    // Wait until serial connects
    delay(10);
  }
  Serial.println("INFO: Application started! :)");
  // Connect to WiFi
  WiFi.mode(WIFI_STA);  // station mode
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println(
        "ERROR: Failed to connect to WiFi!\nPlease check your ssid and password in "
        "include/config.h!");
    delay(500);
  }
  Serial.print("INFO: Connected to WiFi! My IP Address is ");
  Serial.println(WiFi.localIP());
  // MPU6050
  Wire.setPins(16,17);
  while (!mpu.begin()) {
    Serial.println("ERROR: could not connect to a valid MPU6050 sensor!");
    delay(500);
  }
  Serial.println("INFO: MPU6050 sensor ready!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
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
    request->send(200, "application/json", "{\"acceleration\":{"
                                             "\"x\":" +
                                             String(acc.acceleration.x) +
                                             ",\"y\":" +
                                             String(acc.acceleration.y) +
                                             ",\"z\":" +
                                             String(acc.acceleration.z) +
                                             "},\"gyro\":{"
                                             "\"x\":" +
                                             String(gyro.gyro.x) +
                                             ",\"y\":" +
                                             String(gyro.gyro.y) +
                                             ",\"z\":" +
                                             String(gyro.gyro.z) +
                                             "},\"temp\":" +
                                             String(temp.temperature) +
                                             "}");
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}