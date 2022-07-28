#include "driver/mcpwm.h"

#define SET_A_DUTY(duty_cycle) \
  mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, ((duty_cycle)))
#define SET_B_DUTY(duty_cycle) \
  mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, ((duty_cycle)))
#define SET_SERVO_DUTY(duty_cycle) \
  mcpwm_set_duty(MCPWM_UNIT_1, MCPWM_TIMER_1, MCPWM_OPR_A, ((duty_cycle)))

#define MCPWM_START_0() mcpwm_start(MCPWM_UNIT_0, MCPWM_TIMER_0)
#define MCPWM_STOP_0() mcpwm_stop(MCPWM_UNIT_0, MCPWM_TIMER_0)
#define MCPWM_START_1() mcpwm_start(MCPWM_UNIT_1, MCPWM_TIMER_1)
#define MCPWM_STOP_1() mcpwm_stop(MCPWM_UNIT_1, MCPWM_TIMER_1)

void setup() {
  // motor
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, 26);
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, 27);

  mcpwm_config_t motor_pwm_cfg = {
      .frequency = 2000,
      .cmpr_a = 0,                       // A‘s init duty cycle, %
      .cmpr_b = 0,                       // B's init duty cycle, %
      .duty_mode = MCPWM_DUTY_MODE_0,    // Hi mode
      .counter_mode = MCPWM_UP_COUNTER,  // 上位计数
  };

  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &motor_pwm_cfg);

  // servo
  mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM1A, 13);

  mcpwm_config_t servo_pwm_cfg = {
      .frequency = 50,
      .cmpr_a = 0,                       // A‘s init duty cycle, %
      .duty_mode = MCPWM_DUTY_MODE_0,    // Hi mode
      .counter_mode = MCPWM_UP_COUNTER,  // 上位计数
  };

  mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_1, &servo_pwm_cfg);
  SET_SERVO_DUTY(7.5);  // mid pos
  MCPWM_START_1();
}

void loop() {
  // Go straight
  SET_A_DUTY(40);
  SET_B_DUTY(0);
  SET_SERVO_DUTY(7.5);
  delay(5000);
  // Turn left
  SET_A_DUTY(40);
  SET_B_DUTY(0);
  SET_SERVO_DUTY(2.5);
  MCPWM_START_0();
  delay(5000);
  // Go back
  SET_A_DUTY(0);
  SET_B_DUTY(40);
  SET_SERVO_DUTY(7.5);
  delay(5000);
  // Turn back right
  MCPWM_STOP_0();
  SET_A_DUTY(0);
  SET_B_DUTY(40);
  MCPWM_START_0();
  SET_SERVO_DUTY(12.5);
  delay(5000);
}
