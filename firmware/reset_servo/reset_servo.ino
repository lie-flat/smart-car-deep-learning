#include <lie-flat.h>

void setup() {
  init_servo(servo);
  set_servo(7.5);  // mid pos
  servo_start_pwm();
  Serial.println("Successfully reset the servo position.");
}

void loop() {}
