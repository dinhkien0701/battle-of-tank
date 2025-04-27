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

1. [Nhân vật (Xe tăng người chơi)](https://github.com/dinhkien0701/battle-of-tank/blob/main/source_code/ui/co_che.h#L23-L30)
* Nhân vật trong trò chơi được định danh thông qua id
  
* [attribute = 2] : đây là thuộc tính được khởi tạo để phân biệt nó với các đối tượng khác

* [angle]: Xe tăng luôn bắt đầu ở hướng 270 độ (hướng Bắc), xác định góc mà xe tăng di chuyển hoặc bắn.

* [clock]: Cơ chế bắn của nhân vật tích đủ 15 khung hình (FPS) để thực hiện một lần bắn, mang lại cảm giác hành động liên tục nhưng có kiểm soát.

* [defense]: Giáp bảo vệ, giúp xe tăng chống chịu sát thương.

* [Vị trí (rect)]: Được lưu trữ trong một hình chữ nhật SDL (SDL_Rect), đại diện cho tọa độ và kích thước trên màn hình.

2. [Địch (Xe tăng địch)](https://github.com/dinhkien0701/battle-of-tank/blob/main/source_code/ui/co_che.h#L31-L43) :Đối với xe tăng địch, OBJ thêm vào các hành vi độc đáo và thông minh:

* [attribute = 1] : Định danh đây là một đối tượng địch.

* [rand_shot] : Cơ chế bắn ngẫu nhiên được khởi tạo, với thời gian bắn định kỳ mỗi 2 giây, tạo nên sự bất ngờ và thách thức.

* [Hướng di chuyển - angle ] : Địch được khởi tạo với góc 270 độ và thời gian thay đổi hướng thông qua clock, sinh ngẫu nhiên để đảm bảo hành động linh hoạt.

* [Vị trí (rect)] : Tọa độ và kích thước của địch được định nghĩa giúp dễ dàng hiển thị và kiểm tra va chạm với các đối tượng khác.

3. [Vật cản (Tường)](https://github.com/dinhkien0701/battle-of-tank/blob/main/source_code/ui/co_che.h#L45-L53) :Tường đóng vai trò như chướng ngại vật trong trò chơi

* [attribute = 3]: Thuộc tính xác định đây là tường.

* [defense] : Tường bắt đầu với giáp là 3, yêu cầu người chơi hoặc địch phải phá hủy để di chuyển qua.

* [rect] : Định nghĩa vị trí và kích thước của tường trên màn hình.

* => Vai trò: Tường thêm chiều sâu chiến thuật bằng cách cản trở di chuyển của các đối tượng.

4. [Đạn](https://github.com/dinhkien0701/battle-of-tank/blob/main/source_code/ui/co_che.h#L54-L79)  ( Đạn được sinh ra từ cả nhân vật và địch để tạo ra hành động )

* [rect]  : Kích thước nhỏ gọn (10x10) và tọa độ khởi tạo dựa trên vị trí và góc của đối tượng bắn.

* [angle] : Hướng của viên đạn phụ thuộc hoàn toàn vào góc bắn của đối tượng tạo ra nó.

* [attribute] : Loại đạn được liên kết với loại đối tượng (ví dụ: đạn của địch khác với đạn của người chơi) .

5. [Các chức năng hỗ trợ](https://github.com/dinhkien0701/battle-of-tank/blob/main/source_code/ui/co_che.h#L81-L103)
   
* [xoa_obj()] : Xóa đối tượng khỏi trò chơi bằng cách đặt id = -1, biểu thị rằng đối tượng đã bị loại bỏ ( gần như không dùng :]] )

* [tiep_xuc()] : Kiểm tra va chạm giữa hai đối tượng, từ xe tăng chạm vào tường cho đến đạn chạm vào địch.

* [print_obj()] : Hiển thị đối tượng lên màn hình với góc xoay và vị trí chính xác.

### [Khởi tạo bản đồ đối tượng - xem code](https://github.com/dinhkien0701/battle-of-tank/blob/main/source_code/ui/co_che.cpp)

#### Xây dựng bản đồ ban đầu:
Hàm `map_khoi_dong` đảm nhận việc tạo ra một bản đồ gồm các đối tượng như nhân vật chính, địch, và tường. Mỗi đối tượng được định vị theo level hiện tại, giúp game tăng độ khó theo thời gian.

**Các bước tạo bản đồ bao gồm:**
- **Cấp thấp (Level < 3):** Vị trí nhân vật chính và địch được định nghĩa cụ thể. Tường được đặt tại các vị trí cố định, dễ dàng cho người chơi làm quen.
- **Cấp độ cao (Level ≥ 4):** Vị trí nhân vật chính và địch được sinh ngẫu nhiên, đảm bảo sự đa dạng. Tường được khởi tạo với số lượng tăng theo level, tối đa 260 bức.

#### Phân bố đối tượng ngẫu nhiên:
Hàm `dfs_map` được sử dụng để sinh các bức tường một cách ngẫu nhiên, đảm bảo tính chiến thuật và thử thách khi di chuyển. Cơ chế này giúp bản đồ trở nên sống động và không lặp lại.

- **Logic kiểm tra khoảng cách:** Đảm bảo nhân vật chính và địch không bị khởi tạo quá gần nhau, giúp người chơi có không gian để di chuyển và chiến đấu.

#### Gán đối tượng vào bản đồ:
Sau khi bản đồ được tạo, các đối tượng (nhân vật, địch, và tường) được khởi tạo và thêm vào danh sách đối tượng tương ứng (`make_enemy`, `make_obj`). Điều này giúp dễ dàng quản lý và hiển thị các đối tượng trong màn chơi.

#### Vai trò và cơ chế nâng cấp:
- **Nhân vật chính:** Luôn khởi đầu với góc 270 độ và được thưởng thêm mạng mỗi level.
- **Địch:** Tăng số lượng và hành vi linh hoạt khi level cao.
- **Tường:** Số lượng tường tăng theo level, đòi hỏi người chơi phải phá hủy để mở đường di chuyển.

#### Điểm nổi bật của cơ chế khởi tạo:
- **Sự ngẫu nhiên:** Mỗi màn chơi mang lại trải nghiệm mới, không lặp lại, giúp người chơi không nhàm chán.
- **Chiến thuật:** Tường được đặt thông minh để tạo thử thách, kết hợp với vị trí kẻ địch để người chơi phải suy nghĩ kỹ trước khi hành động.
- **Hiệu quả:** Cách tổ chức mã nguồn giúp quản lý đối tượng dễ dàng, mở rộng logic cho các cấp độ khó hơn.

