# Tank Of Battle
 * Link demo game :
 * Phá đảo game : chưa có
## Giới thiệu
 Tựa game 2D đơn giản , người chơi sẽ điều khiển xe tăng để tiêu diệt xe tăng địch nhằm đạt điểm cao nhất có thể

## Hướng dẫn cài đặt game 
### [Truy cập vào Releases để tải game ](https://github.com/dinhkien0701/battle-of-tank/releases)

* BattleOfTank_Setup.exe : Tải game về cài đặt và chơi luôn
* Source.Code.to.Build.App.zip : Phiên bản chứa cả file Setup để cài game kèm mã nguồn đã được thay đổi để có thể tạo ứng dụng hoàn chỉnh
* Battle.Of.Tank.zip : phiên bản mã nguồn gốc của game , chưa tùy biến và có thể chơi 


## Hướng dẫn chơi game
* Mỗi người chơi sẽ bắt đầu với 3 mạng , việc vượt qua mỗi level sẽ giúp người chơi  tăng thêm một mạng 
* Người chơi có thể điều khiển di chuyển theo bốn hướng  W(lên) , S (xuống) , A(trái) , D(phải) để di chuyển ( có thể dùng phím mũi tên để di chuyển )
* Sử dụng phím SPACE để  bắn đạn tấn công địch
* Nhấn phím P để  TẠM DỪNG / TIẾP TỤC chơi game 
* Nhấn phím M để thoát khỏi ván đấu , trở về màn hình  MENU
* Bị đạn địch trúng sẽ mất một mạng , khi số mạng về 0 sẽ coi như kết thúc ván đấu
* Đạn của nhân vật trúng địch sẽ  tiêu diệt kẻ địch đó , các bức tường chịu tối đa 3 lần trúng đạn , sau khi trúng đạn lần thứ 3 tường sẽ sập!

# Cơ chế của game

- [Đối tượng](#Đối-tượng)
- [Luật chơi](#luật-chơi)

## Đối tượng
### [Cấu trúc của đối tượng - xem code](https://github.com/dinhkien0701/battle-of-tank/blob/main/source_code/ui/co_che.h#L9-L103)
1. Nhân vật (Xe tăng người chơi)
* Nhân vật trong trò chơi được định danh thông qua id
  
* [attribute = 2] : đây là thuộc tính được khởi tạo để phân biệt nó với các đối tượng khác

* [angle]: Xe tăng luôn bắt đầu ở hướng 270 độ (hướng Bắc), xác định góc mà xe tăng di chuyển hoặc bắn.

* [clock]: Cơ chế bắn của nhân vật tích đủ 15 khung hình (FPS) để thực hiện một lần bắn, mang lại cảm giác hành động liên tục nhưng có kiểm soát.

* [defense]: Giáp bảo vệ, giúp xe tăng chống chịu sát thương.

* [Vị trí (rect)]: Được lưu trữ trong một hình chữ nhật SDL (SDL_Rect), đại diện cho tọa độ và kích thước trên màn hình.

2. Địch (Xe tăng địch) :Đối với xe tăng địch, OBJ thêm vào các hành vi độc đáo và thông minh:

* [attribute = 1] : Định danh đây là một đối tượng địch.

* [rand_shot] : Cơ chế bắn ngẫu nhiên được khởi tạo, với thời gian bắn định kỳ mỗi 2 giây, tạo nên sự bất ngờ và thách thức.

* [Hướng di chuyển - angle ] : Địch được khởi tạo với góc 270 độ và thời gian thay đổi hướng thông qua clock, sinh ngẫu nhiên để đảm bảo hành động linh hoạt.

* [Vị trí (rect)] : Tọa độ và kích thước của địch được định nghĩa giúp dễ dàng hiển thị và kiểm tra va chạm với các đối tượng khác.

