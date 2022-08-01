#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <config.h>
#include <lie-flat.h>
#include <constants.h>

AsyncWebServer server(80);

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
  Serial.println("Application started! :)");
  // Connect to WiFi
  WiFi.mode(WIFI_STA);  // station mode
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println(
        "Failed to connect to WiFi!\nPlease check your ssid and password in "
        "include/config.h!");
    return;
  }
  Serial.print("Connected to WiFi! My IP Address is ");
  Serial.println(WiFi.localIP());
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
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}