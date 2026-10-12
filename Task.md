# Phân công Lab 01 Xv6 User Programs và File System

## 1. Mục tiêu và phạm vi

Nhóm gồm 3 thành viên, thực hiện 7 chương trình `sleep`, `pingpong`, `primes`, `cp`, `tree`, `du`, `diff`; kiểm thử, viết báo cáo PDF và đóng gói bài nộp theo đề `CQ_Lab01.docx`.

Môi trường đã lựa chọn: Ubuntu 24.04 trên WSL2, source `xv6-labs-2024`, nhánh gốc `util`, trình biên dịch RISC-V và QEMU. Đây là phương án môi trường của nhóm dựa trên tài liệu MIT được đề dẫn tới; nếu giảng viên cung cấp source riêng, nhóm cần đối chiếu trước khi tiếp tục.

Tên TV1–TV3 là placeholder để thay bằng tên và MSSV thực tế. Chưa có deadline cụ thể, vì vậy tiến độ được chia theo mốc bàn giao.

## 2. Bảng phân công tổng thể

| Thành viên | Họ tên và MSSV | Chương trình | File phụ trách | Trách nhiệm chung |
|---|---|---|---|---|
| TV1 | Đăng Khoa | `sleep`, `pingpong`, `primes` | `user/sleep.c`, `user/pingpong.c`, `user/primes.c` | Tích hợp `Makefile`, ghép source, build bản chung và đóng gói bài nộp |
| TV2 | Bảo Trân | `cp`, `diff` | `user/cp.c`, `user/diff.c` | Tổng hợp và biên tập báo cáo PDF |
| TV3 | Nhựt Duy | `tree`, `du` | `user/tree.c`, `user/du.c` | Tổng hợp checklist kiểm thử chung và kết quả kiểm thử chéo |

TV1 có 3 chương trình nhưng `sleep` ngắn; phần chính là quản lý tiến trình và pipeline của `primes`. TV2 tập trung vào đọc/ghi và xử lý dữ liệu file. TV3 tập trung vào duyệt thư mục và đệ quy. Mỗi người chịu trách nhiệm cả code, kiểm thử và giải thích giải pháp của phần mình.

## 3. Công việc của TV1

### T1.1 — Chương trình sleep

**File:** `user/sleep.c` · **Cú pháp:** `sleep ticks`

- [x] Kiểm tra đối số; thiếu đối số phải in lỗi.
- [x] Dùng `atoi` để chuyển chuỗi sang số ticks.
- [x] Gọi system call `sleep` của xv6.
- [x] Kết thúc thành công bằng `exit(0)`.
- [x] Kiểm thử `sleep 10`, `sleep 0` và gọi `sleep` không có đối số.

**Hoàn thành khi:** chương trình chờ theo ticks, trả lại shell và báo lỗi đúng khi thiếu đối số. Nhóm có thể bổ sung kiểm tra chuỗi không hợp lệ/số âm, nhưng không thay thế các yêu cầu bắt buộc trên.

### T1.2 — Chương trình pingpong

**File:** `user/pingpong.c` · **Cú pháp:** `pingpong`

- [x] Tạo 2 pipe, mỗi pipe phục vụ một chiều trao đổi.
- [x] Dùng `fork` để tạo tiến trình con.
- [x] Cha gửi 1 byte; con đọc và in `<pid>: received ping`.
- [x] Con gửi lại 1 byte; cha đọc và in `<pid>: received pong`.
- [x] Đóng các đầu pipe không sử dụng trong cả cha và con.
- [x] Cha chờ con kết thúc; xử lý lỗi syscall cần thiết.
- [x] Kiểm thử nhiều lần, xác nhận không treo và luôn in ping trước pong.

**Hoàn thành khi:** trao đổi đúng 1 byte mỗi chiều, PID đúng tiến trình và cả hai kết thúc. Không hardcode PID theo output mẫu.

### T1.3 — Chương trình primes

**File:** `user/primes.c` · **Cú pháp:** `primes`

- [x] Tiến trình đầu phát các số nguyên từ 2 đến 280.
- [x] Xây sàng dạng pipeline bằng `pipe` và `fork`; mỗi số nguyên tố tương ứng một tiến trình lọc.
- [x] Truyền số nguyên 4 byte qua pipe; in `prime <number>`.
- [x] Chỉ tạo tầng lọc mới khi còn dữ liệu cần xử lý.
- [x] Đóng FD không cần thiết để EOF truyền được và không cạn tài nguyên.
- [x] Mỗi tầng chờ tiến trình con; tiến trình chính chỉ thoát khi toàn bộ pipeline hoàn tất.
- [x] Đối chiếu danh sách kết quả với danh sách số nguyên tố độc lập trong khoảng 2–280.
- [x] Kiểm thử lặp lại, xác nhận không thiếu/trùng số và không treo sau dòng cuối.

**Hoàn thành khi:** triển khai đúng pipeline concurrent, không dùng một tiến trình đơn thay cho yêu cầu; in đủ số nguyên tố và trở lại shell.

### T1.4 — Tích hợp và đóng gói

**Bàn giao TV1:** xem `docs/tv1/README.md`, `grade-tv1` và `tools/package-tv1.py`. Các mục dưới đây áp dụng cho bản chung đủ 7 chương trình; không đánh dấu hoàn thành khi còn thiếu source TV2/TV3.

- [x] Ghi lại commit gốc trên nhánh `util` làm baseline cho cả nhóm. Hash: `79e8024d61c3dd92ccf064fc67bb99cb999f70ab` (xem `docs/tv1/BASELINE`).
- [ ] Thêm đủ 7 chương trình vào `UPROGS` trong `Makefile`.
- [ ] Ghép các phần bàn giao, build và chạy trên source chung.
- [ ] Sinh patch so với baseline; kiểm tra patch chứa cả các file mới.
- [ ] Chạy `make clean` trước khi tạo ZIP source cuối cùng.
- [ ] Đóng gói đúng tên và cấu trúc đề yêu cầu; chuyển cả nhóm kiểm tra trước khi nộp.

## 4. Công việc của TV2

### T2.1 — Chương trình cp

**File:** `user/cp.c` · **Cú pháp:** `cp src dst`

- [ ] Kiểm tra số đối số và in hướng dẫn sử dụng khi sai.
- [ ] Mở nguồn read-only; mở đích để ghi, tạo mới hoặc ghi đè.
- [ ] Copy theo buffer, ví dụ 512 hoặc 1024 bytes.
- [ ] Bảo đảm ghi đủ dữ liệu đọc được; xử lý trường hợp ghi chưa đủ buffer.
- [ ] Khi ghi đè file dài bằng file ngắn, không để dữ liệu cũ ở cuối file đích.
- [ ] Báo lỗi mở/đọc/ghi; đóng FD trong cả nhánh thành công và nhánh lỗi.
- [ ] Kiểm thử file rỗng, file nhỏ, file lớn hơn buffer, nguồn không tồn tại và ghi đè.

**Hoàn thành khi:** nội dung và kích thước file đích khớp nguồn; lỗi được báo rõ. Nhóm nên bổ sung kiểm tra nguồn và đích cùng file trước khi truncate để tránh mất dữ liệu.

### T2.2 — Chương trình diff

**File:** `user/diff.c` · **Cú pháp:** `diff file1 file2 [-q]`

- [ ] Đọc hai file theo từng dòng và so sánh bằng xử lý chuỗi phù hợp.
- [ ] Dòng giống nhau không được in; hai file giống hệt thì không in gì.
- [ ] Dòng khác nhau: nội dung file1 có tiền tố `<`, nội dung file2 có tiền tố `>`.
- [ ] Ghi tên file và số dòng theo format thống nhất của nhóm.
- [ ] Nếu một file EOF trước, tiếp tục xử lý phần còn lại của file kia.
- [ ] Với `-q`, chỉ in `diff: files differ` một lần nếu có khác biệt.
- [ ] Hỗ trợ cả `diff f1 f2 -q` và `diff -q f1 f2`, vì đề có hai cách viết.
- [ ] Không cắt dòng dài một cách im lặng; chọn cách xử lý rõ ràng và ghi giới hạn nếu còn hạn chế.
- [ ] Kiểm thử file rỗng, hai file giống nhau, khác giữa file, khác số dòng, dòng dài và dòng cuối không có newline.

**Hoàn thành khi:** đáp ứng so sánh theo vị trí dòng ở mức cơ bản của đề. Không cần triển khai thuật toán tìm chuỗi con chung dài nhất hoặc diff tối ưu như GNU diff.

### T2.3 — Báo cáo chung

- [ ] Nhận nội dung giải pháp và kiểm thử từ TV1, TV3.
- [ ] Viết phần `cp`, `diff` và tổng hợp thành báo cáo ngắn gọn.
- [ ] Nêu khó khăn, giới hạn, yêu cầu chưa hoàn thành nếu có.
- [ ] Giữ cách dùng thuật ngữ và format nhất quán; không đưa toàn bộ source code vào báo cáo.
- [ ] Xuất PDF, kiểm tra hiển thị và gửi cả nhóm duyệt nội dung kỹ thuật.

## 5. Công việc của TV3

### T3.1 — Chương trình tree

**File:** `user/tree.c` · **Cú pháp:** `tree [path] [-L depth] [-d]`

- [ ] Dùng `.` làm path mặc định.
- [ ] Duyệt thư mục bằng `open`/`read`, dùng `fstat` để phân biệt loại entry.
- [ ] Duyệt đệ quy nhưng bỏ qua `.` và `..`.
- [ ] In path gốc và cấu trúc con bằng một style cố định.
- [ ] Với `-L 1`, chỉ in gốc và các mục con trực tiếp.
- [ ] Với `-d`, chỉ in thư mục.
- [ ] Kiểm tra đối số, giới hạn đường dẫn và lỗi mở/đọc; đóng FD đầy đủ.
- [ ] Kiểm thử thư mục rỗng, cây nhiều cấp, path sai, `-L 1`, `-L 2`, `-d`, kết hợp `-L` và `-d`.

**Hoàn thành khi:** cây đúng độ sâu và đúng bộ lọc, không lặp vô hạn. Nhóm chọn dùng khoảng trắng thụt lề theo cấp cho bản cơ bản; style này được đề cho phép.

### T3.2 — Chương trình du

**File:** `user/du.c` · **Cú pháp:** `du [path] [-a] [-s]`

- [ ] Dùng `.` làm path mặc định; duyệt đệ quy và bỏ `.`/`..`.
- [ ] Lấy dung lượng file từ `st.size`, đơn vị bytes.
- [ ] Cộng dồn dung lượng các file trong toàn bộ cây con.
- [ ] Mặc định chỉ in thư mục; `-a` in cả file và thư mục.
- [ ] Với `-s`, chỉ in tổng của path được chọn.
- [ ] Dùng format `<bytes>\t<path>`; chọn kiểu số đủ lớn cho tổng dung lượng.
- [ ] Đóng FD, kiểm tra giới hạn đường dẫn và báo lỗi truy cập rõ ràng.
- [ ] Kiểm thử thư mục rỗng, một file, cây nhiều cấp, `-a`, `-s` và kết hợp hai option.

**Hoàn thành khi:** tổng đúng dữ liệu thực tế và quy ước nhất quán. Quy ước nội bộ: không cộng `st.size` của bản thân thư mục; thư mục rỗng có tổng 0; khi có cả `-a` và `-s`, `-s` ưu tiên và chỉ in tổng.

**Lưu ý kiểm thử:** số bytes minh họa trong đề có chỗ không khớp nội dung file mẫu. Dùng kích thước thực tế làm căn cứ; không hardcode số mẫu. Không mặc định `echo` trong xv6 hỗ trợ `-e` như một số shell Linux.

### T3.3 — Tổng hợp kiểm thử

- [ ] Thu danh sách test và kết quả từ cả ba thành viên.
- [ ] Tổng hợp test bắt buộc, test biên và các lỗi còn mở.
- [ ] Theo dõi kiểm thử chéo; chỉ đánh dấu đạt khi có kết quả thực tế.
- [ ] Viết phần `tree`, `du`, giải thích quy ước dung lượng và kết quả kiểm thử.

## 6. Quy ước chung trước khi viết code

| Nội dung | Quy ước nội bộ |
|---|---|
| Branch làm việc | TV1: `lab01-process`; TV2: `lab01-files`; TV3: `lab01-directories` |
| Baseline | Cùng một commit gốc của `util`; lưu hash hoặc tag chung trước khi sửa |
| Phạm vi sửa | Mỗi người sửa file được giao; thay đổi file chung phải báo TV1 để tích hợp |
| Makefile | TV1 quản lý bản chung; TV2/TV3 bàn giao danh sách entry cần thêm, có thể thêm tạm ở nhánh riêng để tự test |
| Kernel | Không sửa kernel nếu chưa có lý do và thống nhất của nhóm |
| Header và thư viện | Dùng API trong source xv6; không giả định có đầy đủ libc của Linux |
| Lỗi | In ra stderr bằng API xv6, nêu tên chương trình và lý do |
| Exit status | Thông thường: 0 thành công, 1 lỗi; riêng `diff`: 0 giống, 1 khác, 2 lỗi thực thi |
| Tài nguyên | Đóng FD và chờ tiến trình con khi cần, kể cả nhánh lỗi |
| Build | Chạy trong terminal Ubuntu/WSL ở thư mục repo |
| Chạy chương trình | Chạy trong shell xv6 sau `make qemu` |
| Kiểm thử MIT | Dùng cho phần tương ứng có sẵn; không coi `make grade` là kiểm tra đủ 7 bài của môn |

Các quy ước trên là lựa chọn của nhóm, không phải tất cả đều là yêu cầu bắt buộc của đề. Nếu test của giảng viên quy định format hoặc exit status cụ thể, ưu tiên quy định đó.

## 7. Mốc triển khai và bàn giao

| Mốc | TV1 | TV2 | TV3 | Điều kiện chuyển mốc |
|---|---|---|---|---|
| M1 — Chốt môi trường | Chốt baseline và kiểm tra build | Chạy được source gốc | Chạy được source gốc | Cả nhóm cùng source và khởi động được xv6 |
| M2 — Code và tự test | Hoàn thành nhóm process | Hoàn thành nhóm file | Hoàn thành nhóm thư mục | Mỗi chương trình có source và kết quả tự test |
| M3 — Ghép và review | Tích hợp source/Makefile; review phần TV2 | Review phần TV3 | Review phần TV1; tổng hợp lỗi | Bản chung build được, các lỗi chính đã sửa |
| M4 — Báo cáo và đóng gói | Tạo patch/source ZIP và gói cuối | Hoàn thiện report PDF | Chốt checklist kiểm thử | Cả nhóm duyệt đủ code, test, report và ZIP |

**Kiểm thử chéo:** TV1 kiểm tra `cp`/`diff`; TV2 kiểm tra `tree`/`du`; TV3 kiểm tra `sleep`/`pingpong`/`primes`. Người review báo lỗi với lệnh tái hiện và kết quả mong đợi; người phụ trách sửa và chạy lại test liên quan.

### Nội dung mỗi người phải bàn giao

- Source đã commit trong nhánh riêng và hash commit bàn giao.
- Danh sách chương trình/entry cần thêm vào `UPROGS`.
- Lệnh tạo dữ liệu, lệnh chạy test, kết quả mong đợi và kết quả thực tế.
- Giải thích ngắn về thuật toán, syscall sử dụng và quản lý tài nguyên.
- Danh sách giới hạn hoặc lỗi chưa xử lý, nếu có.
- Nội dung phần báo cáo của mình.

## 8. Checklist nghiệm thu và nộp bài

- [ ] Bản chung build thành công, khởi động được và có đủ 7 chương trình.
- [ ] Các test chức năng và trường hợp biên đã được chạy trên bản chung.
- [ ] `pingpong`/`primes` không treo; file/thư mục được xử lý đúng quy ước.
- [ ] Báo cáo có bảng đóng góp từng người, giải pháp, kết quả test và giới hạn thực tế.
- [ ] PDF ngắn gọn, đọc được, không chứa toàn bộ mã nguồn.
- [ ] Patch được tạo so với baseline và chứa đầy đủ file mới/thay đổi cần nộp. File mới phải được Git theo dõi trước khi sinh patch; chỉ dùng `git diff` sau commit có thể làm thiếu phần đã commit.
- [ ] Thử áp patch lên một bản source sạch cùng baseline và kiểm tra build.
- [ ] Source được chạy `make clean` trước khi ZIP. Hoàn tất lưu bằng chứng test trước bước này vì ảnh filesystem có thể bị tạo lại ở lần build sau.
- [ ] Tên ZIP và report dùng MSSV theo thứ tự tăng dần, không dùng TV1–TV3.
- [ ] Cả nhóm kiểm tra nội dung gói cuối; nộp trên MOODLE.

### Cấu trúc gói nộp

Với nhóm 3 người, dùng `StudentID1 < StudentID2 < StudentID3`:

```text
StudentID1_StudentID2_StudentID3.zip
  StudentID1_StudentID2_StudentID3_Report.pdf
  Realease/
    StudentID1_StudentID2_StudentID3.patch
  Source/
    xv6-labs-2024.zip
```

Giữ cách viết `Realease` theo đề gốc; đối chiếu hướng dẫn nộp trên MOODLE nếu giảng viên có cập nhật cấu trúc.

## 9. Bảng theo dõi công việc

| Task | Owner | Code | Tự test | Review chéo | Báo cáo | Ghi chú |
|---|---|---|---|---|---|---|
| sleep | TV1 | Hoàn thành | Hoàn thành | Chờ TV3 | Đã bàn giao nội dung | `docs/tv1/README.md` và log |
| pingpong | TV1 | Hoàn thành | Hoàn thành | Chờ TV3 | Đã bàn giao nội dung | 50 lần chạy liên tiếp |
| primes | TV1 | Hoàn thành | Hoàn thành | Chờ TV3 | Đã bàn giao nội dung | 20 lần, đủ 59 số nguyên tố |
| cp | TV2 | Chưa làm | Chưa làm | TV1 | Chưa làm | |
| diff | TV2 | Chưa làm | Chưa làm | TV1 | Chưa làm | |
| tree | TV3 | Chưa làm | Chưa làm | TV2 | Chưa làm | |
| du | TV3 | Chưa làm | Chưa làm | TV2 | Chưa làm | |
| Tích hợp và đóng gói | TV1 | Đang làm | Đạt phần TV1 | Chờ cả nhóm | Không áp dụng | Đã thêm 3 entry; có script tạo/kiểm tra patch và ZIP bàn giao; chờ source TV2/TV3, PDF và MSSV |
| Tổng hợp report PDF | TV2 | Không áp dụng | Không áp dụng | Cả nhóm | Chưa làm | |
| Tổng hợp test | TV3 | Không áp dụng | Chưa làm | Cả nhóm | Chưa làm | |

Trạng thái đề xuất: `Chưa làm`, `Đang làm`, `Chờ review`, `Cần sửa`, `Hoàn thành`.

## 10. Tài liệu tham khảo

- Đề bài: `CQ_Lab01.docx` do nhóm cung cấp.
- Lab MIT được đề tham khảo: https://pdos.csail.mit.edu/6.1810/2024/labs/util.html
- Hướng dẫn công cụ: https://pdos.csail.mit.edu/6.1810/2024/tools.html
- Source sử dụng: `git://g.csail.mit.edu/xv6-labs-2024`, nhánh `util`.
