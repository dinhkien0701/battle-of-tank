#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <cmath>
#include <string>
#include <utility>  // Thêm thư viện này để sử dụng std::pair
#include <algorithm>
#include <thread>
#include <atomic> //giúp đọc/ghi biến một cách nguyên tử (atomic operation), đảm bảo không bị gián đoạn khi có luồng khác truy cập.
#include "co_che.h"

std::atomic<bool> running(true);  // Kiểm soát vòng lặp , đảm bao gpu hoạt động trơn chu


void heart(int &cx, int &cy ,int x, int y, int w, int h){
    //Tìm tâm của đối tượng
    cx=static_cast<int>(x+0.5*w);
    cy=static_cast<int>(y+0.5*h);
}
double to_radian(int angle){
    //Quy đổi từ độ ra radian
    return angle/180.0*3.1415;
}
int to_degree(double angle){
    //quy đổi từ radian ra độ
    return static_cast<int>(angle/3.1415*180);
}
void xoay_vector(int &ansX, int &ansY ,int x, int y, int cx, int cy , int angle ){
    //xoay (x,y) quanh (cx,cy) một gốc angle
    double radian = to_radian(angle); // quy đổi từ độ ra radian
    ansX = cx + static_cast<int>((x-cx)*cos(radian)-(y-cy)*sin(radian));//xoay diem x quanh cx
    ansY = cy + static_cast<int>((x-cx)*sin(radian)+(y-cy)*cos(radian));//xoay diem y quanh cy
}
void mapping(int &ansX, int &ansY , int cx, int cy , int px , int py , double scaleX, double scaleY){
        //Ánh xạ điểm từ png sang vị trí nó hiển thị trên màn hình
        //ansX và ansY : điểm của ảnh sau khi xoay bởi hàm xoay_vector
        // cx,cy : tâm của ảnh png bị xoay ( ảnh xoay quanh tâm này )

        ansX = static_cast<int> ( scaleX*ansX);
        ansY = static_cast<int> (scaleY*ansY);

        //Ánh xạ ( biến đổi ) ansX, ansY là điểm của pixel ảnh hiển thị tại vị trí trên màn hình
}
void layer(SDL_Renderer *renderer, SDL_Surface *surface, TextureInfo & info, pixelmp **mp){

    /*Tải ảnh lên Surface
    SDL_Surface *surface = IMG_Load("path.png");

    */
    Uint32 *pixels =(Uint32*)surface->pixels; // tạo con trỏ đến bảng màu của ảnh
    int pitch = surface->pitch / sizeof(Uint32);  // Số pixel trên mỗi dòng (có padding)



    int width = surface->w; //chiều rộng ảnh
    int height = surface->h; // chiều cao ảnh

    int cx,cy; // tâm của ảnh png
    int px, py;// tâm của đối tượng trên khung hình
    heart(cx,cy,0,0,width,height); // tinh cx,cy
    heart(px,py,info.rect.x,info.rect.y,info.rect.w,info.rect.h); // tính px, py
    int ansX, ansY ;// biến dể lưu điểm ảnh qua ánh xạ kép xoay_vector và mapping
    Uint8 r,b,g,a ;// màu và độ trong
   // Duyệt qua từng pixel trong ảnh png
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
                Uint32 pixel = pixels[j*pitch+i]; // tính chỉ số của pixel ( mảng hai chiều của pixels được đưa vè một chiều )
                SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a); // lấy thông tin pixel ảnh

                // Ánh xạ ansX, ansY hai lần để có được vị trí hiện thị của điểm ảnh lên màn hình
                xoay_vector(ansX,ansY,i,j,cx,cy,info.angle);
                mapping(ansX,ansY,cx,cy,px,py,1.0*info.rect.w/width,1.0*info.rect.h/height);
                if(a>0){
                    //Tồn tại tiếp xúc giữa vật thể với môi trường
                    info.scale =std::min(info.scale,mp[ansX][ansY].speed);
                    mp[ansX][ansY].speed=info.scale;
                    info.defense-=mp[ansX][ansY].damage;
                    mp[ansX][ansY].damage=std::max(mp[ansX][ansY].damage,info.damage);

                }
        }
    }
}

bool sol(SDL_Surface *surface, TextureInfo &info, pixelmp **mp){
    //kiểm tra vị trí này có thể đặt đối tượng được không




    Uint32 *pixels =(Uint32*)surface->pixels; // tạo con trỏ đến bảng màu của ảnh
    int pitch = surface->pitch / sizeof(Uint32);  // Số pixel trên mỗi dòng (có padding)

    int width = surface->w; //chiều rộng ảnh
    int height = surface->h; // chiều cao ảnh

    int cx,cy; // tâm của ảnh png
    int px, py;// tâm của đối tượng trên khung hình
    heart(cx,cy,0,0,width,height); // tinh cx,cy
    heart(px,py,info.rect.x,info.rect.y,info.rect.w,info.rect.h); // tính px, py
    int ansX, ansY ;// biến dể lưu điểm ảnh qua ánh xạ kép xoay_vector và mapping
    Uint8 r,b,g,a ;// màu và độ trong
   // Duyệt qua từng pixel trong ảnh png
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
                Uint32 pixel = pixels[j*pitch+i]; // tính chỉ số của pixel ( mảng hai chiều của pixels được đưa vè một chiều )
                SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a); // lấy thông tin pixel ảnh

                // Ánh xạ ansX, ansY hai lần để có được vị trí hiện thị của điểm ảnh lên màn hình
                xoay_vector(ansX,ansY,i,j,cx,cy,info.angle);
                mapping(ansX,ansY,cx,cy,px,py,1.0*info.rect.w/width,1.0*info.rect.h/height);
                if(a>0&&(ansX<1||ansX>780)&&(ansY<1||ansY>780))return false;
                if(a>0&&mp[ansX][ansY].alpha){
                    //Tồn tại tiếp xúc giữa vật thể với môi trường
                    return false;
                }
        }
    }
    return true;
}

