# Solar-Tracker
🌞 Hệ thống Solar Tracker sử dụng ESP32 và nền tảng ERa IoT

Hệ thống Solar Tracker là một giải pháp thông minh giúp tự động điều chỉnh góc nghiêng của tấm pin năng lượng mặt trời nhằm tối ưu hoá lượng ánh sáng thu được trong ngày. Hệ thống sử dụng bo mạch chính ESP32 và được kết nối với nền tảng ERa IoT để giám sát và điều khiển từ xa qua internet.

<h1>⚙️ Thành phần chính:</h1>

ESP32: Bộ vi điều khiển mạnh mẽ, hỗ trợ kết nối WiFi, dùng làm trung tâm điều khiển và giao tiếp với các cảm biến, motor và nền tảng đám mây.

4 cảm biến LDR (Light Dependent Resistor): Được gắn theo các hướng để đo cường độ ánh sáng, từ đó xác định hướng có ánh sáng mạnh nhất.

2 servo motor: Dùng để xoay tấm pin theo trục ngang (East-West) và trục dọc (Up-Down), đảm bảo luôn hướng về phía có ánh sáng tối ưu.

Cảm biến INA219: Đo điện áp, dòng điện và công suất từ tấm pin mặt trời, phục vụ cho việc theo dõi hiệu suất hệ thống.

Module ERa IoT: Giúp gửi dữ liệu như điện áp, dòng điện, công suất, trạng thái pin, vị trí motor,... lên nền tảng ERa, từ đó người dùng có thể giám sát qua điện thoại hoặc trình duyệt web.

<h1> 🌐 Tính năng chính:</h1>

Tự động dò ánh sáng mặt trời và điều chỉnh góc của tấm pin để thu được năng lượng tối đa.

Gửi dữ liệu thời gian thực lên ERa IoT, bao gồm:

Cường độ ánh sáng

Góc xoay của motor

Điện áp và dòng điện từ pin

Trạng thái hệ thống

Giám sát và điều khiển từ xa qua điện thoại hoặc giao diện web của ERa.

Chế độ tiết kiệm năng lượng, tự động tắt servo khi không cần thiết.

<h1>📈 Lợi ích:</h1>

Tối đa hoá hiệu suất thu năng lượng mặt trời.

Tiết kiệm chi phí bảo trì và giám sát nhờ khả năng theo dõi từ xa.

Dễ mở rộng, lập trình linh hoạt với mã nguồn ESP32.

Tương thích với các hệ thống năng lượng mặt trời dân dụng hoặc học thuật.
