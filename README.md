# FARC 2025 - IronCortex Team

**Thử thách Nông Nghiệp Bền Vững | FPTU AI & Robotics Challenge 2025**

---

## 🌾 Giới thiệu đội thi

**IronCortex** là đội thi đến từ \[trường hoặc địa phương] tham dự vòng chung kết **FPTU AI & Robotics Challenge 2025**.

* **Thành viên đội:**

  | Họ tên     | Vai trò                           |
  | ---------- | --------------------------------- |
  | \[Nguyễn Khải Huyền] | Đội trưởng|
  | \[Ngô Xuân Hải]   | Phụ trách cơ khí  |
  | \[Nguyễn Ngọc Hoàng Xuân Hòa]   | Human Player, vận hành|
  | \[Lê Duy Mạnh, Lương Hiếu Ngân]   | Lập trình |

---

## 🌟 Mục tiêu và chiến lược thi đấu

### Bài toán:

* Vận chuyển **Đất** và **Hạt giống** vào Nhà kính để thu hoạch **Nông sản**
* Cân bằng Đòn gánh Nông sản
* Leo cột cân bằng năng lượng trong 60 giây cuối
* Kích hoạt **Mùa bội thu** để ghi điểm tối đa

### Chiến lược:

1. **Giai đoạn đầu:**

   * Robot 1: Chuyên chở Đất và Hạt giống vào Nhà kính
   * Robot 2: Lấy Nông sản và mang về Đòn gánh

2. **Giai đoạn cuối:**

   * Hợp tác cân bằng đòn gánh
   * 60 giây cuối: Cả 2 Robot leo cột cân bằng năng lượng
   * Kích hoạt Mùa bội thu

---

## 🛠 Thiết kế kỹ thuật

### Cơ khí:

* Khung nhôm nhẹ, mô-đun tháo lắp nhanh
* Cơ cấu gắp đa năng: Xoay 180°, kẹp nông sản + bóng
* Cơ cấu leo cột: 2 móc kẹp với motor 545 và bánh răng chuyển động song song

### Điện tử:

* Mạch điều khiển **VIA B ESP32**
* Pin Lipo 3S 12V 3000mAh
* Động cơ: 4 motor 545 + 2 Servo MG995 (gắp) + 2 Servo MG996R (leo cột)
* Cảm biến: TCRT5000, cảm biến màu TCS34725, cảm biến khoảng cách HC-SR04

### Phần mềm:

* Điều khiển tay cầm không dây qua controller
* Hệ thống Auto-assist: Hỗ trợ căn chỉnh khi leo cột
* Hỗ trợ chuyển trạng thái tự động giữa các pha thi đấu

---

## ⚙️ Sơ đồ hệ thống

![System Diagram](https://via.placeholder.com/800x400?text=System+Diagram+IronCortex)

---

## 📂 Cấu trúc thư mục

```
├── docs/                   # Tài liệu kỹ thuật (Design, Engineering)
├── cad/                    # File thiết kế CAD robot
├── hardware/               # BOM, vật liệu, sơ đồ mạch điện
├── software/               # Code điều khiển robot
├── images/                 # Hình ảnh minh họa, video link
├── README.md               # Giới
```
