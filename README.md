# Ổ CẮM ĐIỆN IOT  
Về cơ bản là một ổ cắm điện có khả năng đo dòng điện và điện áp, công suất. Khả năng điều khiển từ xa thông qua app/web hoặc ra lệnh bằng giọng nói.  
Mục tiêu của dự án thực hiện đồ án môn học, cũng như muốn ứng dụng platform Arduino Cloud vào trong đồ án.  
Nâng cao kỹ năng bản thân và làm việc với nhóm.  

## TÍNH NĂNG CHÍNH:  
- Bật/tắt cung cấp nguồn điện 220VAC.  
- Đo dòng điện/điện áp công suất thiệt bị được cấp điện.  
- Ra lệnh bằng giọng nói, chuyển động thông qua app Arduino IoT Cloud Remote.  
  
## PHẦN CỨNG CẦN THIẾT:  
| Tên Linh Kiện | Ghi Chú | Số Lượng |
| :--- | :--- | :---: |
| STM32F103C8T6 | Mạch điều khiển chính | 1 |
| ESP32 | Mạch trao đổi thông tin | 1 |  
| ZMPT101B | Đo điện áp AC | 1 |
| ZMCT103C | Mạch Cảm Biến Dòng AC 5A | 1 |
| HLK-PM01 | Chuyển Đổi AC-DC 220-5V 3W | 1 |
| AMS1117 | Mạch ổn áp 1.2V cung cấp nguồn vi điều khiển | 1 | 
| BTA16-600BRG | TRIAC bật/tắt nguồn điện 220VAC | 1 |
| MOC3041M | Cách ly vi điều khiển với TRIAC | 1 |

## SƠ ĐỒ KẾT NỐI: (Rất tiếc đã làm mất nhưng về cơ bản)  
- ZMPT101B mắc vời mạch cầu chia áp vào vi điều khiển.  
- ZMCT103C mắc theo datasheet.  
- HLK-PM01 mắc trực tiếp vào ESP32 qua mạch ổn áp vào STM32.  
- STM32 và ESP32 giao tiếp UART.  
- BTA16-6000BRG mắc với MOC3041M mắc theo datasheet.  
  
## PHẦN MỀM VÀ THƯ VIỆN:  
### 1. Môi trường:  
Cài đặt cấu hình phần cứng: STM32CubeMX.  
Môi trường phát triển: Keil C, Arduino Cloud.   

### 2. Cấu hình:  
Thật sự lâu rồi mình không còn nhớ cách để cấu hình. Mình nhớ chính là hai file các bạn cần lưu tâm là:  
- `O_CAM_DIEN_DA_NANG_oct12a.ino` của ESP32 ưu tiên chỉnh sữa trên WEB Arduino Cloud.  
- `main.h` của STM32F103C8T6 bạn có thể xem code và điều chỉnh trong đó.  

### 3. Biên dịch và Nạp code:  
Trên trang Arduino Cloud để biên dịch nạp code ESP32.  
Sử dụng Keil C để nạp STM32F103C8T6.  

## Cách sử dụng:  
[Video hướng dẫn](https://youtu.be/Entkjo6MZXI)  

## Các thành viên trong nhóm:
Trần Quốc Cường  
Đào Thế Anh  
Trần Lê Hiền Thục  
Nguyễn Thị Thanh Nhàn

## Liên hệ:
Tên: Trần Quốc Cường  
Email: tranquoccuong103111@gmail.com  
GitHub: github.com/TranQuoc-Cuong
  



