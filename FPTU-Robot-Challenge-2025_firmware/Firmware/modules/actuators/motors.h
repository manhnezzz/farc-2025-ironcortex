#include <stdio.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define MIN_PWM 0
#define MAX_PWM 4095

// PWM channels of pca9685 0-16
#define PWM_CHANNEL1 8
#define PWM_CHANNEL2 9
#define PWM_CHANNEL3 10
#define PWM_CHANNEL4 11

#define PWM_CHANNEL5 12
#define PWM_CHANNEL6 13

#define PWM_CHANNEL7 14
#define PWM_CHANNEL8 15

//
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
int motorPosition_nghiengrobot = 0; // 0: gốc, 1: lên 90 độ, -1: xuống 90 độ

void setPWMMotors(int c1, int c2, int c3, int c4) {
  char dbg_str[30];
  sprintf(dbg_str, "C1: %d\tC2: %d\tC3: %d\tC4: %d", c1, c2, c3, c4);
  Serial.println(dbg_str);

  pwm.setPin(PWM_CHANNEL1, c1);
  pwm.setPin(PWM_CHANNEL2, c2);
  pwm.setPin(PWM_CHANNEL3, c3);
  pwm.setPin(PWM_CHANNEL4, c4);
}

//Khỏi động mềm cho motors
int currentPWM1 = 0, currentPWM2 = 0, currentPWM3 = 0, currentPWM4 = 0;
const int SOFT_START_STEP = 500;  // điều chỉnh nhanh/chậm

void setPWMMotorsSoftStart(int target1, int target2, int target3, int target4) {
  if (currentPWM1 < target1) currentPWM1 = min(currentPWM1 + SOFT_START_STEP, target1);
  else if (currentPWM1 > target1) currentPWM1 = max(currentPWM1 - SOFT_START_STEP, target1);

  if (currentPWM2 < target2) currentPWM2 = min(currentPWM2 + SOFT_START_STEP, target2);
  else if (currentPWM2 > target2) currentPWM2 = max(currentPWM2 - SOFT_START_STEP, target2);

  if (currentPWM3 < target3) currentPWM3 = min(currentPWM3 + SOFT_START_STEP, target3);
  else if (currentPWM3 > target3) currentPWM3 = max(currentPWM3 - SOFT_START_STEP, target3);

  if (currentPWM4 < target4) currentPWM4 = min(currentPWM4 + SOFT_START_STEP, target4);
  else if (currentPWM4 > target4) currentPWM4 = max(currentPWM4 - SOFT_START_STEP, target4);

  setPWMMotors(currentPWM1, currentPWM2, currentPWM3, currentPWM4);
}

void setPWMMotors_nangha(int c1, int c2){
  pwm.setPin(PWM_CHANNEL5, c1);
  pwm.setPin(PWM_CHANNEL6, c2);
}

void setPWMMotors_nghiengrobot(int c1, int c2){
  pwm.setPin(PWM_CHANNEL7, c1);
  pwm.setPin(PWM_CHANNEL8, c2);
}

void initMotors() {
  Wire.begin();  // SDA, SCL,400000);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(1600);
  Wire.setClock(400000);

  setPWMMotors(0, 0, 0, 0);
  setPWMMotors_nangha(4095, 4095);
}
