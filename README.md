# Tank Of Battle
 * Link demo game :
 * Phá đảo game : chưa có
## Giới thiệu
 Tựa game 2D đơn giản , người chơi sẽ điều khiển xe tăng để tiêu diệt xe tăng địch nhằm đạt điểm cao nhất có thể

## Hướng dẫn cài đặt game 
### [Truy cập vào Releases để tải game ](https://github.com/dinhkien0701/battle-of-tank/releases)
   Ta có 3 lựa chọn
   BattleOfTank_Setup.exe :

## Hướng dẫn chơi game
* Mỗi người chơi sẽ bắt đầu với 3 mạng , việc vượt qua mỗi level sẽ giúp người chơi  tăng thêm một mạng 
* Người chơi có thể điều khiển di chuyển theo bốn hướng  W(lên) , S (xuống) , A(trái) , D(phải) để di chuyển ( có thể dùng phím mũi tên để di chuyển )
* Sử dụng phím SPACE để  bắn đạn tấn công địch
* Nhấn phím P để  TẠM DỪNG / TIẾP TỤC chơi game 
* Nhấn phím M để thoát khỏi ván đấu , trở về màn hình  MENU
* Bị đạn địch trúng sẽ mất một mạng , khi số mạng về 0 sẽ coi như kết thúc ván đấu
* Đạn của nhân vật trúng địch sẽ  tiêu diệt kẻ địch đó , các bức tường chịu tối đa 3 lần trúng đạn , sau khi trúng đạn lần thứ 3 tường sẽ sập!


Game được cấu tạo nhờ  việc sử dụng thuật toán chính yếu là DFS để tạo bản đồ và BFS để tạo Simple AI cho địch 
Game sử dụng SDL2 bao gồm cả mixer/image/ttf


Nguồn ảnh : AI , cắt từ một số game , vẽ một số hình đơn giản + sử dụng https://www.remove.bg/vi để xóa nền tạo png
Nguồn nhạc : nhạc nền ( Tiny Troopers ) ;  âm thanh đạn , tiếng nổ : https://tiengdong.com/
Nguồn font chữ : google font OpenSans


Link giới thiệu + cơ chế + demo  https://www.youtube.com/watch?v=PYt8heMRYs4
