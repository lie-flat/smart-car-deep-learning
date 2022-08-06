#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncUDP.h>
#include <constants.h>

// Be sure to define the WiFi mode before include lie-flat.h
#define LIE_FLAT_WIFI_STA
#include <lie-flat.h>

AsyncWebServer server(80);
Adafruit_MPU6050 mpu;
AsyncUDP udp;
IPAddress camAddr;

#define FRAME_HEADER "lie-flat device discovery!"
const char* FRAME_ACK = FRAME_HEADER "\nACK";

void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

inline __attribute__((always_inline)) float parse_float_param(
    AsyncWebServerRequest* request,
    const char* param) {
  return request->getParam(param, true)->value().toFloat();
}

inline __attribute__((always_inline)) int parse_int_param(
    AsyncWebServerRequest* request,
    const char* param) {
  return request->getParam(param, true)->value().toInt();
}

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
      set_servo(parse_float_param(request, SERVO_PARAM));
    if (request->hasParam(MOTOR_A_PARAM, true))
      set_a(parse_float_param(request, MOTOR_A_PARAM));
    if (request->hasParam(MOTOR_B_PARAM, true))
      set_b(parse_float_param(request, MOTOR_B_PARAM));
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
  server.on("/buzz", HTTP_POST, [](AsyncWebServerRequest* request) {
    int freq = 3000;
    int duration = 1000;
    if (request->hasParam(FREQ_PARAM, true))
      freq = parse_int_param(request, FREQ_PARAM);
    if (request->hasParam(DURATION_PARAM, true))
      duration = parse_int_param(request, DURATION_PARAM);
    Serial.printf("INFO: Buzzing! Freq: %d, duration: %d\n", freq, duration);
    buzz(freq, duration);
    request->send(200, "text/plain", "OK");
  });
  server.onNotFound(notFound);
  server.begin();
  if (udp.listen(1234)) {
    Serial.print("UDP Listening on IP: ");
    Serial.println(WiFi.localIP());
    udp.onPacket([](AsyncUDPPacket packet) {
      if (packet.length() > 512) {
        Serial.println("WARN: Dropping extra large packet!");
        return;
      }
      if (strncmp((char*)packet.data(), FRAME_HEADER, strlen(FRAME_HEADER)) != 0) {
        Serial.println("WARN: Not a lie-flat frame");
        return;
      }
      
      Serial.print("UDP Packet Type: ");
      Serial.print(packet.isBroadcast()   ? "Broadcast"
                   : packet.isMulticast() ? "Multicast"
                                          : "Unicast");
      Serial.print(", From: ");
      Serial.print(packet.remoteIP());
      Serial.print(":");
      Serial.print(packet.remotePort());
      Serial.print(", To: ");
      Serial.print(packet.localIP());
      Serial.print(":");
      Serial.print(packet.localPort());
      Serial.print(", Length: ");
      Serial.print(packet.length());
      Serial.print(", Data: ");
      Serial.write(packet.data(), packet.length());
      Serial.println();
      // Parse lie-flat device discovery frame
      auto rframe = String(packet.data(), packet.length());
      int sep1 = rframe.indexOf('\n');
      int sep2 = rframe.indexOf('\n', sep1+1);
      if (sep1 < 0 || sep2 < 0) return;
      auto verb = rframe.substring(sep1+1, sep2);
      if (verb != "Advertise") return;
      int sep3 = rframe.indexOf('\n', sep2+1);
      if (sep3 < 0) return;
      auto ip = rframe.substring(sep2+1, sep3);
      if (!camAddr.fromString(ip) || camAddr != packet.remoteIP()) {
        Serial.println("WARN: Invalid/Corrupted IP address");
        return;
      }
      auto device = rframe.substring(sep3+1);
      if (device != "esp32-cam") {
        Serial.println("WARN: Unknown device " + device);
        return;
      }
      Serial.println("INFO: Found camera at " + camAddr.toString());
      // reply to the client
      packet.print(FRAME_ACK);
    });
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // udp.println("Master!");
  delay(1000);
}