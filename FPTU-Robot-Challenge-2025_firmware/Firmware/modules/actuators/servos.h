#include <stdio.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

//Khai bao cac kenh Servo
const int servoChannel_dk_taygap = 3;     // Kênh điều khiển lên xuống tay gắp
const int servoChannel_taygap = 4;        // Kênh đóng - mở tay gắp
const int servoChannel_datbong = 5;       //Đẩy bóng vào nhà kính
const int servoChannel_nghiengrobot = 6;  //

// Khai bao cho Servo tay gắp (180 độ)
int pos_taygap = 150;             // Vị trí servo ban đầu
const int servoMin_taygap = 100;  // Giới hạn thấp (khoảng 0 độ)
const int servoMax_taygap = 160;  // Giới hạn cao (khoảng 180 độ)


// Khai báo Servo bỏ (đặt) bóng (180 độ)
int pos_datbong = 3;               // Vị trí servo ban đầu
const int servoMin_datbong = 3;    // Giới hạn thấp (khoảng 3 độ)
const int servoMax_datbong = 165;  // Giới hạn cao (khoảng 160 độ)


//Khai bao cho Servo điều khiển tay gắp (360 độ)
const int pulseStop_dk_taygap = 330;      // Dừng
const int pulseForward_dk_taygap = 400;   // Quay phải
const int pulseBackward_dk_taygap = 250;  // Quay trái

//Khai bao cho Servo nghiêng Robot (360 độ)
const int pulseStop_nghiengrobot = 330;      // Dừng
const int pulseForward_nghiengrobot = 400;   // Quay phải
const int pulseBackward_nghiengrobot = 250;  // Quay trái
int servoPosition_nghiengrobot = 0; // 0: gốc, 1: lên 90 độ, -1: xuống 90 độ

unsigned long lastMoveTime_taygap = 0;
const int moveInterval_taygap = 1;  // thời gian giữa mỗi bước (ms)

unsigned long lastMoveTime_datbong = 0;
const int moveInterval_datbong = 1;  // thời gian giữa mỗi bước (ms)

Adafruit_PWMServoDriver pwm_servo = Adafruit_PWMServoDriver();

// Hàm đặt góc servo
void setServoAngle(uint8_t channel, int angle) {
  int pulse = map(angle, 0, 180, 150, 600);  // 150-600 tuỳ loại servo
  pwm_servo.setPWM(channel, 0, pulse);
}
void setServo360(int servoChannel, int pulse) {
  pwm_servo.setPWM(servoChannel, 0, pulse);  // 500 quay phải; 200 quay trái; 300 dừng
}
// Chỉnh góc Servo từ từ. Ví dụ moveServoSmooth(servoNum, 90, 0, 10);  // Di chuyển từ 90° về 0° mượt mà
void moveServoSmooth(int servo, int fromDeg, int toDeg, int delayTime) {
  if (fromDeg > toDeg) {
    for (int i = fromDeg; i >= toDeg; i -= 4) {
      int pulse = map(i, 0, 180, 150, 600);
      pwm.setPWM(servo, 0, pulse);
      delay(delayTime);
    }
  } else {
    for (int i = fromDeg; i <= toDeg; i += 4) {
      int pulse = map(i, 0, 180, 150, 600);
      pwm.setPWM(servo, 0, pulse);
      delay(delayTime);
    }
  }
}
void initServos() {
  pwm_servo.begin();
  pwm_servo.setPWMFreq(50);                          // Tần số điều khiển servo 50Hz
  setServoAngle(servoChannel_taygap, pos_taygap);    // Đặt vị trí ban đầu
  setServoAngle(servoChannel_datbong, pos_datbong);  // Đặt vị trí ban đầu
  setServo360(servoChannel_dk_taygap, pulseStop_dk_taygap);
  //
}