/*
 * A better version of MakerBotwPS2 example used with makerbot BANHMI hardware platform
 * This version simplify the joystick driving method, where the functionality stay the same.
 * Added new controling menthod, switch driving mode by pressing SELECT button
 * Added nitro like speed booster when holding L2
 * 
 * Writen by Tu Dang - Makerviet
 */
#include <esp_system.h>
#include <esp_attr.h>
#include "motors.h"
#include "servos.h"
#include "cambien.h"
#include "PS2_controller.h"

RTC_DATA_ATTR int resetCount = 0;

void setup() {
  Serial.begin(115200);
  delay(500);  // Chờ ổn định

  Serial.println("Dang khoi dong...");

  if (!setupPS2controller()) {
    resetCount++;
    if (resetCount >= 3) {
      Serial.println("Da reset 3 lan. Dung chuong trinh.");
      while (1)
        ;  //Dung han, khong reset nua.
    } else {
      Serial.println("Loi tay cam PS2. Reset lại sau 0.5 giây..");
      delay(500);
      esp_restart();
    }
  }
  //Neu den day khong co loi thi => reset bien ve 0
  resetCount = 0;
  initMotors();
  initServos();
  initCamBienVatCan();
  Serial.println("Hoan tat setup!");
}


void loop() {
  ps2x.read_gamepad(0, 0);
  PS2control();
  delay(50);
}
