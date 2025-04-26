;--------------------------------------------------
; Battle of Tank Installer Script
;--------------------------------------------------

; Đặt tên và thông tin trình cài đặt
Name "Battle of Tank Installer"
OutFile "BattleOfTank_Setup.exe"

; Gắn icon cho trình cài đặt (chỉ ảnh hưởng đến installer)
Icon "image\icon.ico"

; Thư mục cài đặt mặc định
InstallDir "$PROGRAMFILES\Battle Of Tank"

; Yêu cầu quyền admin để cài đặt
RequestExecutionLevel admin

;--------------------------------------------------
; Phần cài đặt Game
;--------------------------------------------------
Section "Install Game" SecGame
    ; Đặt thư mục cài đặt là thư mục gốc ($INSTDIR)
    SetOutPath "$INSTDIR"

    ; Cài đặt file thực thi và các DLLs
    File "Battle_Of_Tank.exe"
    File "SDL2.dll"
    File "SDL2_image.dll"
    File "SDL2_ttf.dll"
    File "SDL2_mixer.dll"
    File "libgcc_s_seh-1.dll"
    File "libstdc++-6.dll"
    File "libwinpthread-1.dll"

    ; Cài đặt thư mục font
    SetOutPath "$INSTDIR\font"
    File /r "font\*.*"

    ; Cài đặt thư mục music
    SetOutPath "$INSTDIR\music"
    File /r "music\*.*"

    ; Cài đặt thư mục image
    SetOutPath "$INSTDIR\image"
    File /r "image\*.*"

    ; Cài đặt thư mục lưu điểm (point)
    SetOutPath "$INSTDIR\point"
    File /r "point\*.*"

    ; Tạo file Uninstaller bên trong thư mục cài đặt
    WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

;--------------------------------------------------
; Phần tạo Shortcut (Desktop và Start Menu)
;--------------------------------------------------
Section "Create Shortcuts" CreateShortcutsSec ; Đặt tên ID cho section để dễ quản lý (ví dụ: dùng trong uninstaller)

    ; Tạo thư mục cho shortcut trong Start Menu (nếu chưa có)
    CreateDirectory "$SMPROGRAMS\Battle Of Tank"

    ; --- Sửa lỗi ở đây ---
    ; Đặt thư mục làm việc (working directory) cho các shortcut sắp tạo.
    ; Đây là cách đúng để đảm bảo game tìm thấy các file cần thiết trong thư mục cài đặt khi chạy từ shortcut.
    SetOutPath "$INSTDIR"

    ; Tạo shortcut trong Start Menu
    ; Tham số thứ 7 (hotkey) bây giờ để trống: ""
    ; Tham số thứ 8 (description - mô tả) có thể thêm vào cho rõ ràng hơn (tùy chọn)
    CreateShortCut "$SMPROGRAMS\Battle Of Tank\Battle Of Tank.lnk" "$INSTDIR\Battle_Of_Tank.exe" "" "$INSTDIR\image\icon.ico" 0 SW_SHOWNORMAL "" "Chạy Battle Of Tank"

    ; Tạo shortcut trên Desktop
    ; Tương tự, hotkey trống (""). Thư mục làm việc đã được SetOutPath ở trên đặt rồi.
    CreateShortCut "$DESKTOP\Battle Of Tank.lnk" "$INSTDIR\Battle_Of_Tank.exe" "" "$INSTDIR\image\icon.ico" 0 SW_SHOWNORMAL "" "Chạy Battle Of Tank"

SectionEnd

;--------------------------------------------------
; Phần gỡ cài đặt (Uninstall)
;--------------------------------------------------
Section "Uninstall"
    ; Xóa các shortcut đã tạo
    Delete "$SMPROGRAMS\Battle Of Tank\Battle Of Tank.lnk"
    RMDir "$SMPROGRAMS\Battle Of Tank"
    Delete "$DESKTOP\Battle Of Tank.lnk"
    
    ; Xóa file Uninstaller và thư mục cài đặt
    Delete "$INSTDIR\Uninstall.exe"
    RMDir /r "$INSTDIR"
SectionEnd

