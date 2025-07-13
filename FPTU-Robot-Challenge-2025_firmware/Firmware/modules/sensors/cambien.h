// Khai báo chân
const int trigPin_VatCan = 32;
const int echoPin_VatCan = 36;  //màu vàng

#define IR_SENSOR_PIN_nghiengrobot 25


float distance_VatCan;

// Ngưỡng khoảng cách để nhận diện vật cản (cm)
const int thresholdDistance_VatCan = 20;

// Thời gian timeout cho pulseIn (đơn vị: micro giây)
const unsigned long TIMEOUT_US = 30000;  // ~30m khoảng cách tối đa

unsigned long lastCheckVatCan = 0;
bool vatCanDetected = false;
const unsigned long intervalVatCanCheck = 200;  // kiểm tra mỗi 200ms



void initCamBienVatCan() {
  pinMode(trigPin_VatCan, OUTPUT);    // Chân trig là OUTPUT
  pinMode(echoPin_VatCan, INPUT);     // Chân echo là INPUT
  digitalWrite(trigPin_VatCan, LOW);  // Đảm bảo trig ở mức thấp
  pinMode(IR_SENSOR_PIN_nghiengrobot, INPUT);
}

// Hàm kiểm tra vật cản trong khoảng cách cho trước
bool checkVatCan(int khoangCach) {
  // Gửi xung Trigger
  digitalWrite(trigPin_VatCan, LOW);
  delayMicroseconds(2);  // Đảm bảo mức thấp
  digitalWrite(trigPin_VatCan, HIGH);
  delayMicroseconds(10);  // Gửi xung 10μs
  digitalWrite(trigPin_VatCan, LOW);

  // Đọc thời gian Echo với giới hạn timeout
  long duration = pulseIn(echoPin_VatCan, HIGH, TIMEOUT_US);
  if (duration == 0) {
    // Không nhận được tín hiệu Echo (quá xa hoặc lỗi)
    //Serial.println("Khong nhan duoc tin hieu Echo.");
    return false;
  }

  // Tính khoảng cách (đơn vị: cm)
  distance_VatCan = duration * 0.034 / 2;

  // In khoảng cách (tuỳ chọn, có thể giới hạn tần suất in để đỡ tốn thời gian)
  Serial.print("Vat can o khoang cach: ");
  Serial.print(distance_VatCan, 1);  // 1 chữ số sau dấu phẩy
  Serial.println(" cm");

  // Kiểm tra điều kiện
  return (distance_VatCan <= khoangCach);
}
// hàm trả về giá trị speed theo tuyến tính khoảng cách đến vật cản
int getSpeedFromDistance(float distance, int SLOW_SPEED, int MAX_SPEED, float SAFE_DISTANCE_MIN, float SAFE_DISTANCE_MAX) {
  if (distance <= SAFE_DISTANCE_MIN) {
    return SLOW_SPEED;
  } else if (distance >= SAFE_DISTANCE_MAX) {
    return MAX_SPEED;
  } else {
    // Tuyến tính giữa SLOW_SPEED và MAX_SPEED
    float ratio = (distance - SAFE_DISTANCE_MIN) / (SAFE_DISTANCE_MAX - SAFE_DISTANCE_MIN);
    float speed = SLOW_SPEED + (MAX_SPEED - SLOW_SPEED) * ratio;
    return round(speed);  // Làm tròn về int vì analogWrite cần int (0-255)
  }
}
