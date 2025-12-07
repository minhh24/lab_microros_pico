\# Lab: Giao tiếp hai chiều ROS 2 và micro-ROS trên Raspberry Pi Pico (UART)



\*\*Tác giả:\*\* Nguyễn Quang Minh

\*\*Liên hệ:\*\* 0916254336



---



\## 1. Giới thiệu

Dự án này là bài thực hành (Lab) mẫu về việc thiết lập giao tiếp hai chiều giữa máy tính chạy \*\*ROS 2 (Ubuntu)\*\* và vi điều khiển \*\*Raspberry Pi Pico\*\* sử dụng \*\*micro-ROS\*\*.



\*\*Điểm đặc biệt:\*\*

Source code này sử dụng \*\*Hardware UART Driver\*\* (truy cập trực tiếp thanh ghi phần cứng) thay vì thư viện `stdio` mặc định. Điều này giúp giải quyết triệt để các vấn đề:

\* Mất kết nối hoặc trễ gói tin khi chạy trên Máy ảo (VirtualBox/VMware).

\* Đảm bảo độ ổn định cao cho các ứng dụng thời gian thực (như xe tự cân bằng).



\*\*Mục tiêu Lab:\*\*

1\.  \*\*Publisher:\*\* Pico gửi dữ liệu đếm số lên topic `/pico\_data`.

2\.  \*\*Subscriber:\*\* Pico nhận lệnh từ topic `/led\_cmd` để bật/tắt đèn LED.



---



\## 2. Chuẩn bị Phần cứng



\### Linh kiện cần thiết

1\.  \*\*Raspberry Pi Pico\*\* (x1)

2\.  \*\*Module USB-to-TTL\*\* (CP2102, CH340, hoặc FTDI...) (x1)

3\.  \*\*Dây nối\*\* (Jumper wires)



\### Sơ đồ nối dây (Wiring Diagram)

Kết nối module USB-to-TTL với Raspberry Pi Pico theo bảng sau:



| Chân Module CP2102 | Chân Raspberry Pi Pico | Chức năng |

| :--- | :--- | :--- |

| \*\*RX\*\* | \*\*GP0 (Pin 1)\*\* | Pico phát dữ liệu (TX) |

| \*\*TX\*\* | \*\*GP1 (Pin 2)\*\* | Pico thu dữ liệu (RX) |

| \*\*GND\*\* | \*\*GND (Pin 3 hoặc 38)\*\* | Mass chung (Bắt buộc) |

| \*\*5V/VCC\*\* | \*\*VSYS (Pin 39)\*\* | Cấp nguồn (Nếu không cắm cáp MicroUSB) |



> \*\*Lưu ý:\*\* Nếu bạn vẫn cắm cáp Micro USB vào Pico để nạp code hoặc lấy nguồn, chỉ cần nối 3 dây: \*\*RX, TX, GND\*\*.



---



\## 3. Cài đặt và Biên dịch (Build)



\### Yêu cầu phần mềm

\* Ubuntu 22.04 (ROS 2 Humble) hoặc tương đương.

\* Đã cài đặt `micro\_ros\_agent`.

\* Đã cài đặt `pico-sdk`.



\### Các bước thực hiện



\*\*Bước 1: Clone dự án về máy\*\*

```bash

git clone <link-github-cua-ban>

cd lab\_microros\_pico

