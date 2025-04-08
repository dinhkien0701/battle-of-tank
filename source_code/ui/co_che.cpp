#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <cmath>
#include <iostream>
#include <string>
#include <set>
#include <utility>  // Thêm thư viện này để sử dụng std::pair
#include <algorithm>
#include <thread>
#include <fstream>
#include <atomic> //giúp đọc/ghi biến một cách nguyên tử (atomic operation), đảm bảo không bị gián đoạn khi có luồng khác truy cập.
#include "co_che.h"
#include "act.h"
using namespace std;

std::atomic<bool> running(true);  // Kiểm soát vòng lặp , đảm bao gpu hoạt động trơn chu

int distance(int &x1,int &y1,int &x2, int &y2){
    return static_cast<int>(sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)));
}
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

        ansX = px+static_cast<int> ( scaleX*(ansX-cx));
        ansY = py+static_cast<int> ( scaleY*(ansY-cy));

        //Ánh xạ ( biến đổi ) ansX, ansY là điểm của pixel ảnh hiển thị tại vị trí trên màn hình
}
void layer( SDL_Surface *surface, TextureInfo & info, pixelmp **mp){

    /*Tải ảnh lên Surface
    SDL_Surface *surface = IMG_Load("path.png");

    */
    Uint32 *pixels =(Uint32*)surface->pixels; // tạo con trỏ đến bảng màu của ảnh
    int pitch = surface->pitch / sizeof(Uint32);  // Số pixel trên mỗi dòng (có padding)

    int sat_thuong_phai_chiu =0;


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
                if(a>30){

                    mp[ansX][ansY].initialize(info.id,info.attribute);
                }
                else if(a >0 ) mp[ansX][ansY].initialize(info.id,info.attribute);
        }
    }
}

bool sol(SDL_Surface *surface, SDL_Rect &new_rect,int id,int angle , pixelmp **mp){
    //kiểm tra vị trí này có thể đặt đối tượng được không




    Uint32 *pixels =(Uint32*)surface->pixels; // tạo con trỏ đến bảng màu của ảnh
    int pitch = surface->pitch / sizeof(Uint32);  // Số pixel trên mỗi dòng (có padding)

    int width = surface->w; //chiều rộng ảnh
    int height = surface->h; // chiều cao ảnh

    int cx,cy; // tâm của ảnh png
    int px, py;// tâm của đối tượng trên khung hình
    heart(cx,cy,0,0,width,height); // tinh cx,cy
    heart(px,py,new_rect.x,new_rect.y,new_rect.w,new_rect.h); // tính px, py
    int ansX, ansY ;// biến dể lưu điểm ảnh qua ánh xạ kép xoay_vector và mapping
    Uint8 r,b,g,a ;// màu và độ trong
   // Duyệt qua từng pixel trong ảnh png
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
                Uint32 pixel = pixels[j*pitch+i]; // tính chỉ số của pixel ( mảng hai chiều của pixels được đưa vè một chiều )
                SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a); // lấy thông tin pixel ảnh

                // Ánh xạ ansX, ansY hai lần để có được vị trí hiện thị của điểm ảnh lên màn hình
                xoay_vector(ansX,ansY,i,j,cx,cy,angle);
                mapping(ansX,ansY,cx,cy,px,py,1.0*new_rect.w/width,1.0*new_rect.h/height);
                if((ansX<0||ansX>1319||ansY<40||ansY>759)&&a>0)return false;
                if(a>30&&mp[ansX][ansY].alpha&&id!=mp[ansX][ansY].alpha){
                    //Tồn tại tiếp xúc giữa vật thể với môi trường
                    return false;
                }
        }
    }
    return true;
}

void mapping_dich( SDL_Surface*surface, TextureInfo & enemy, pixelmp **mp){

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
    heart(px,py,enemy.rect.x,enemy.rect.y,enemy.rect.w,enemy.rect.h); // tính px, py
    int ansX, ansY ;// biến dể lưu điểm ảnh qua ánh xạ kép xoay_vector và mapping
    Uint8 r,b,g,a ;// màu và độ trong
   // Duyệt qua từng pixel trong ảnh png
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
                Uint32 pixel = pixels[j*pitch+i]; // tính chỉ số của pixel ( mảng hai chiều của pixels được đưa vè một chiều )
                SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a); // lấy thông tin pixel ảnh

                // Ánh xạ ansX, ansY hai lần để có được vị trí hiện thị của điểm ảnh lên màn hình
                xoay_vector(ansX,ansY,i,j,cx,cy,enemy.angle);
                mapping(ansX,ansY,cx,cy,px,py,1.0*enemy.rect.w/width,1.0*enemy.rect.h/height);
                if(a>0){
                    if(a<30)mp[ansX][ansY].initialize(enemy.id,-enemy.attribute);
                    else mp[ansX][ansY].initialize(enemy.id,enemy.attribute);

                }
        }
    }
}

bool tuong_tac_dan(SDL_Surface *surface, TextureInfo & info, TextureInfo *tuong ,TextureInfo *dich , TextureInfo &main , pixelmp **mp){

    /*Tải ảnh lên Surface
    SDL_Surface *surface = IMG_Load("path.png");

    */
    bool dan_ton_tai =true;
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
   set<int> st1; // mảng địch
   set<int> st2; // mảng vật cản
   int st_main =0;
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
                Uint32 pixel = pixels[j*pitch+i]; // tính chỉ số của pixel ( mảng hai chiều của pixels được đưa vè một chiều )
                SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a); // lấy thông tin pixel ảnh
                if(a==0){
                    //không va chạm , kiểm tra điểm tiếp theo
                    continue;
                }
                // Ánh xạ ansX, ansY hai lần để có được vị trí hiện thị của điểm ảnh lên màn hình
                xoay_vector(ansX,ansY,i,j,cx,cy,info.angle);
                mapping(ansX,ansY,cx,cy,px,py,1.0*info.rect.w/width,1.0*info.rect.h/height);
                if(ansX<0||ansX>1319||ansY<40 ||ansY > 759){
                    // Đạn đã rời khỏi khung hình , sẽ không tồn tại nữa

                    dan_ton_tai =false;

                }
                else if(mp[ansX][ansY].alpha >0 &&mp[ansX][ansY].attribute!=info.id ){
                    dan_ton_tai = false ;

                    if(info.attribute == 2){
                        // Đối tượng là main
                        st_main =1;
                    }
                    else if(info.id == 1){
                        //Đối tượng là địch
                        st1.insert(mp[ansX][ansY].alpha);
                    }
                    else{
                        //Đối tượng là công trình
                        st2.insert(mp[ansX][ansY].alpha);
                    }
                }
        }
    }
    main.defense -= st_main;
    for(int st : st1){
        dich[st].defense -=1;
    }
    for(int st : st2){
        tuong[st].defense-= info.id;
    }
    return dan_ton_tai ;
}
void dfs_map(int i, int j , int mapp[45][25],int &total , int max_total){
    mapp[i][j]=1;
    int res = 0;
    for(int num = rand()%3 +1; num >0; num--){
        if(res == 2||total>=max_total)continue;
        int x = rand()%2 - rand()%2 ;
        int y = rand()%2 - rand()%2 ;
        if(x<2||x>32||y<2||y>16||mapp[i+x][j+y])continue;
        dfs_map(i+x,j+y,mapp,++total,max_total); // ++total : thêm giá trị mới truyền , total++ có thể lỗi

    }
}
void map_khoi_dong( int level , TextureInfo *make_enemy , TextureInfo *make_obj , TextureInfo &mainn , int &sum_enemy , int &sum_obj){
    // khoiwt tạo mảng ban đầu để random map
    int mapp[45][25];
    for(int i=0;i<45;i++){
        for(int j=0;j<25;j++){
            mapp[i][j]=0;
        }
    }

    /*
      1: tường
      2: địch
      3: main
      */
    if(level <3){
        mapp[20][10]=3; // vị trí ban đầu của người chơi
        mapp[2][2]=2;   // vị trí ban đầu của địch
        for(int i=0;i<= level;i++){
            for(int j=0;j<=level;j++){
                mapp[5*(i+1)][5*(j+1)]=1;
            }
        }
    }

    else {
         if(level <5){
            mapp[30][15]=3; // vị trí ban đầu của người chơi
            mapp[2][2]=mapp[17][2]= mapp[25][4]=2;
         }
         else{
            // level >= 5 ;

            // cx, cy vị trí ô nhân vật chính
            int cx= 2+ rand()%30; // random từ 2 ->31
            int cy= 2+ rand()%15; // random từ 2 ->16;
            mapp[cx][cy]=3;

            for(int i= min(10,3+level/5 + level/13 + level/18);i>=0;i-- ){
                int x,y;

                do{
                    x =2+ rand()%30; // rand từ 2->31
                    y =2+ rand()%15; // rand từ 2->16
                } while(mapp[x][y]>0 || sqrt((cx-x)*(cx-x)+(cy-y)*(cy-y))<9);// rand đến khi tìm đucợ vị trí trống và cách đủ xa
                mapp[x][y] = 2; // vị trí kẻ địch khi random
            }
         }
         int max_total =level*15; // số bức tường tương ứng level
         max_total = min(260,max_total); // tối đa 260 bức tường
         int total =0;
         while(total<max_total){
            int i =2+ rand()%30;  // rand từ 2->31
            int j =2+ rand()%36;  // rand từ 2->16
            if(mapp[i][j]==0){
                dfs_map(i,j,mapp,++total, max_total);
            }
         }
    }
    //lấy các đối tượng đã được tạo
    sum_enemy =10;
    sum_obj   =20;
    for(int i=2;i<32;i++){
        for(int j=2;j<17;j++){
            if(mapp[i][j]>0){
                SDL_Rect rect = {40*i,40*j,40,40};
                if(mapp[i][j]==1){
                    make_obj[sum_obj].khoi_tao_tuong(sum_obj,rect); // tạo chướng ngại vật
                    sum_obj++;
                }
                else if(mapp[i][j]==2){
                    make_enemy[sum_enemy].khoi_tao_dich(sum_enemy,rect);
                    make_enemy[sum_enemy].angle=make_enemy[sum_enemy].angle_two = 270;
                    sum_enemy++;

                }
                else if(mapp[i][j]==3){
                    mainn.khoi_tao_nhan_vat(1,min(3,mainn.defense+1),rect);

                    mainn.angle = mainn.angle_two = 270;
                    //nhân vật thêm một mạng mỗi lượt chơi , tối đa tích trữ 3 mạng
                }

            }
        }
    }

}
