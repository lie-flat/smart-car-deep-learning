#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <config.h>
#include <lie-flat.h>

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
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
  // Connect to WiFi
  WiFi.mode(WIFI_STA);  // station mode
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println(
        "Failed to connect to WiFi!\nPlease check your ssid and password in "
        "include/config.h!");
    return;
  }
  Serial.print("My IP Address is ");
  Serial.println(WiFi.localIP());
  // Web server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/plain", "Hello, world");
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}