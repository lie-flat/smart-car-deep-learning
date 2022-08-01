#include <init.h>
#include <ctrl.h>
#include <pin.h>

void setup() {
  init_motor(motorA, motorB);
  init_servo(servo);
  set_servo(7.5);  // mid pos
  servo_start_pwm();
}

void loop() {
  // Go straight
  set_a(40);
  set_b(0);
  set_servo(7.5);
  delay(5000);
  // Turn left
  set_a(40);
  set_b(0);
  set_servo(2.5);
  motor_start_pwm();
  delay(5000);
  // Go back
  set_a(0);
  set_b(40);
  set_servo(7.5);
  delay(5000);
  // Turn back right
  motor_stop_pwm();
  set_a(0);
  set_b(40);
  motor_start_pwm();
  set_servo(12.5);
  delay(5000);
}
