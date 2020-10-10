# TCP Server

Ứng dụng tách xâu sử dụng TCP Socket Server

**Server:**

- Chạy ở số hiệu cổng bất kì theo theo số dòng lệnh
- Nhận yêu cầu do client gửi lên
- Trả lại kết quả cho client

**Client:**

- Người dùng nhập một xâu từ bàn phím
- Client gửi yêu cầu tới server
- Chức năng lặp lại cho đến khi người dùng nhập vào một xâu rỗng

| **INPUT**          | **OUTPUT**                                   |
| ------------------ | -------------------------------------------- |
| 1a2b3c4d           | Result:<br>1234<br>abcd                      |
| 123@               | Error: String contains special charaacter.   |