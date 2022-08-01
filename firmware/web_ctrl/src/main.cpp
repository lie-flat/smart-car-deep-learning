#include <Arduino.h>
#include <lie-flat.h>

void setup() {
  // put your setup code here, to run once:
  init_motor(motorA, motorB);
  init_servo(servo);
  set_servo(7.5);
  servo_start_pwm();
}

void loop() {
  // put your main code here, to run repeatedly:
  set_servo(2.5);
  delay(3000);
  set_servo(7.5);
  delay(3000);
  set_servo(12.5);
  delay(3000);
}