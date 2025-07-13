#include "PS2X_lib.h"

PS2X ps2x;

// Calibration & PS2 pin config
#define X_JOY_CALIB 127
#define Y_JOY_CALIB 128

#define PS2_DAT 12
#define PS2_CMD 13
#define PS2_SEL 15
#define PS2_CLK 14

#define pressures false
#define rumble false

// Speed settings
#define TOP_SPEED 3000
#define NORM_SPEED 1700 // tốc độ bình thường
#define SLOW_SPEED 600 // tốc độ giảm khi có vật cản

// Driving mode
#define SINGLE_HAND_DRIVING 0
#define TWO_HAND_DRIVING 1
bool driving_mode = SINGLE_HAND_DRIVING;

// Function: Setup PS2 Controller
bool setupPS2controller() {
  Serial.begin(115200);
  Serial.print("Ket noi voi tay cam PS2:");
  int error = -1;

  for (int i = 0; i < 10; i++) {
    delay(1000);
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
    Serial.print(".");
    if (error == 0) break;
  }

  Serial.println();

  switch (error) {
    case 0: Serial.println("Ket noi tay cam PS2 thanh cong"); return true;
    case 1: Serial.println("LOI: Khong tim thay tay cam"); break;
    case 2: Serial.println("LOI: Khong gui duoc lenh"); break;
    case 3: Serial.println("LOI: Khong vao duoc Pressures mode"); break;
    default: Serial.println("LOI: Khong xac dinh");
  }

  return false;
}

// Hàm xử lý điều khiển PS2
bool PS2control() {
  ps2x.read_gamepad();
  if (ps2x.Analog(PSS_RX) == 0xFF && ps2x.Analog(PSS_LY) == 0xFF) {
    Serial.println("Mất kết nối tay cầm!");
    setPWMMotors(0, 0, 0, 0);
    setPWMMotors_nangha(4095, 4095);
    return false;
  }

  // --- QUAY ĐẦU TỰ ĐỘNG 180 ĐỘ BẰNG JOYSTICK TRAI + PSB_BLUE ---
int lx = ps2x.Analog(PSS_LX);  // Đọc giá trị joystick trái ngang
const int JOY_THRESHOLD = 50;  // Ngưỡng nhận dạng lệch joystick
static bool da_quay_dau = false;
if (ps2x.Button(PSB_BLUE)) {
  if (!da_quay_dau) {
    if (lx < (128 - JOY_THRESHOLD)) {
      Serial.println("Quay dau 180 do sang trai (joystick + BLUE)...");
      setPWMMotors(TOP_SPEED, 0, 0, TOP_SPEED);  // Quay trái
      delay(650);
      setPWMMotors(0, 0, 0, 0);
      da_quay_dau = true;
      return true;
    } else if (lx > (128 + JOY_THRESHOLD)) {
      Serial.println("Quay dau 180 do sang phai (joystick + BLUE)...");
      setPWMMotors(0, TOP_SPEED, TOP_SPEED, 0);  // Quay phải
      delay(650);
      setPWMMotors(0, 0, 0, 0);
      da_quay_dau = true;
      return true;
    }
  }
} else {
  da_quay_dau = false;  // Reset lại khi thả nút BLUE
}

  // ==== TAY GẮP XOAY ====
  if (ps2x.Button(PSB_L2))
    setServo360(servoChannel_dk_taygap, pulseForward_dk_taygap);
  else if (ps2x.Button(PSB_PAD_DOWN))
    setServo360(servoChannel_dk_taygap, pulseBackward_dk_taygap);
  else
    setServo360(servoChannel_dk_taygap, pulseStop_dk_taygap);

  // ==== NGHIÊNG ROBOT ====
  int status_nghiengrobot = 1;  //1 là đỉnh; -1 đáy
  if (ps2x.Button(PSB_PINK)) {  //nghiêng xuống
    if (digitalRead(IR_SENSOR_PIN_nghiengrobot) == HIGH) {
      //Serial.println("Khong co gi can robot nghieng");
      setPWMMotors_nghiengrobot(700, 0);
    } else setPWMMotors_nghiengrobot(4095, 4095);
    //setServo360(servoChannel_nghiengrobot, pulseForward_nghiengrobot);
    /*if (motorPosition_nghiengrobot != 1) {
      motorPosition_nghiengrobot = 1;
      setPWMMotors_nghiengrobot(4095, 0);
      delay(200);
      setPWMMotors_nghiengrobot(0, 0);
    }*/
  } else if (ps2x.Button(PSB_RED)) {  // nghiêng lên
                                      //if (digitalRead(IR_SENSOR_PIN_nghiengrobot) == HIGH) {
    //Serial.println("Phat hien can robot nghieng lên");
    setPWMMotors_nghiengrobot(0, 700);
    //}
    //else setPWMMotors_nghiengrobot(4095, 4095);
    //setServo360(servoChannel_nghiengrobot, pulseBackward_nghiengrobot);
    /*if (motorPosition_nghiengrobot != -1) {
      motorPosition_nghiengrobot = -1;
      setPWMMotors_nghiengrobot(0, 4095);
      delay(200);
      setPWMMotors_nghiengrobot(0, 0);
    }*/
  } else setPWMMotors_nghiengrobot(4095, 4095);  //setServo360(servoChannel_nghiengrobot, pulseStop_nghiengrobot);

  // ==== NÂNG HẠ TAY GẮP ====
  if (ps2x.Button(PSB_L1))
    setPWMMotors_nangha(4095, 0);
  else if (ps2x.Button(PSB_PAD_UP))
    setPWMMotors_nangha(0, 4095);
  else
    setPWMMotors_nangha(4095, 4095);

  // ==== TAY GẮP MỞ/ĐÓNG ====
  unsigned long now = millis();
  if (ps2x.Button(PSB_PAD_RIGHT) && now - lastMoveTime_taygap > moveInterval_taygap && pos_taygap < servoMax_taygap) {
    pos_taygap = constrain(pos_taygap +12, servoMin_taygap, servoMax_taygap);
    setServoAngle(servoChannel_taygap, pos_taygap);
    lastMoveTime_taygap = now;
    Serial.print("Servo lên: ");
    Serial.println(pos_taygap);
    delay(10);
  }
  if (ps2x.Button(PSB_PAD_LEFT) && now - lastMoveTime_taygap > moveInterval_taygap && pos_taygap > servoMin_taygap) {
    pos_taygap = constrain(pos_taygap -15, servoMin_taygap, servoMax_taygap);
    setServoAngle(servoChannel_taygap, pos_taygap);
    lastMoveTime_taygap = now;
    Serial.print("Servo xuống: ");
    Serial.println(pos_taygap);
    delay(10);
  }

  // ==== ĐẶT BÓNG ====
  if (ps2x.Button(PSB_GREEN) && pos_datbong >= servoMax_datbong) {
    moveServoSmooth(servoChannel_datbong, servoMax_datbong, servoMin_datbong, 10);
    pos_datbong = servoMin_datbong;
  } else if (ps2x.Button(PSB_R1) && pos_datbong <= servoMin_datbong) {
    moveServoSmooth(servoChannel_datbong, servoMin_datbong, servoMax_datbong, 10);
    pos_datbong = servoMax_datbong;
  }

  // ==== TỐC ĐỘ & CẢM BIẾN VẬT CẢN ====
  int speed = ps2x.Button(PSB_R2) ? TOP_SPEED : NORM_SPEED;

  if (!ps2x.Button(PSB_R2) && pos_taygap == servoMin_taygap) {
    if (millis() - lastCheckVatCan >= intervalVatCanCheck) {
      vatCanDetected = checkVatCan(thresholdDistance_VatCan);
      lastCheckVatCan = millis();
    }
    if (vatCanDetected)
      speed = SLOW_SPEED;
  }

  // ==== CHUYỂN CHẾ ĐỘ LÁI ====
  if (ps2x.ButtonPressed(PSB_SELECT)) driving_mode = !driving_mode;

  // ==== XỬ LÝ DI CHUYỂN ====
  int nJoyX = X_JOY_CALIB - ps2x.Analog(PSS_RX);
  int nJoyY = Y_JOY_CALIB - (driving_mode ? ps2x.Analog(PSS_LY) : ps2x.Analog(PSS_RY));

  if (nJoyX == -1 && nJoyY == 0) {
    setPWMMotors(0, 0, 0, 0);
    return 0;
  }

  // Dead zone
  const int DEAD_ZONE = 10;
  if (abs(nJoyX) < DEAD_ZONE) nJoyX = 0;
  if (abs(nJoyY) < DEAD_ZONE) nJoyY = 0;

  // Arcade Drive Mixing
  int nMotMixL = constrain(nJoyY + nJoyX, -128, 128);
  int nMotMixR = constrain(nJoyY - nJoyX, -128, 128);

#ifdef BEBUG_CTRL
  Serial.print(F("Tốc độ động cơ: "));
  Serial.print(nMotMixL);
  Serial.print("\t");
  Serial.println(nMotMixR);
#endif

  // Map sang PWM
  int c1 = (nMotMixL > 0) ? map(nMotMixL, 0, 128, 0, speed) : 0;
  int c2 = (nMotMixL < 0) ? map(-nMotMixL, 0, 128, 0, speed) : 0;
  int c3 = (nMotMixR > 0) ? map(nMotMixR, 0, 128, 0, speed) : 0;
  int c4 = (nMotMixR < 0) ? map(-nMotMixR, 0, 128, 0, speed) : 0;

  //setPWMMotorsSoftStart(c1, c2, c3, c4);
  setPWMMotors(c1, c2, c3, c4);
  return 1;
}
