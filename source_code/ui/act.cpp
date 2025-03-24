#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <utility>
#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include "co_che.h"
#include "act.h"
bool quit = true;//khi người dùng chưa ấn thoát
SDL_Surface* rbga(int r, int g, int b, int a) {
    // Tạo một surface 50x50 pixel với định dạng 32-bit RGBA
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, 50, 50, 32, SDL_PIXELFORMAT_RGBA8888);
    if (!surface) {
        SDL_Log("SDL_CreateRGBSurfaceWithFormat Error: %s", SDL_GetError());
        return nullptr;
    }

    // Nếu bạn muốn sử dụng thứ tự (r, b, g, a) thì hãy truyền như vậy;
    // Nếu theo thứ tự chuẩn (r, g, b, a), bạn thay đổi các đối số tương ứng.
    Uint32 mappedColor = SDL_MapRGBA(surface->format, r, b, g, a);
    SDL_FillRect(surface, NULL, mappedColor);

    return surface;
}
void resetmp(int w, int h , pixelmp **mp){
    for(int i=0;i<=w;i++){
        for(int j=0;j<=h;j++){
            mp[i][j].initialize(0,1,0,0);
        }
    }
}
void new_obj_location(SDL_Rect &rect , int angle ,int step){
    // vị trí mới sau di chuyển
    double radian = to_radian(angle);
    rect.x += static_cast<int>(step*cos(radian));
    rect.y += static_cast<int>(step*sin(radian));
}
std::pair<int,int> center_obj(const SDL_Rect &rect){
    //Xác định tâm đối tượng
    return {rect.x+ (int)(0.5*rect.w),rect.y+(int)(0.5*rect.h)};
}
int degree(std::pair<int,int> a, std::pair<int,int> b){
    return static_cast<int>(atan(1.0*(b.first -a.first)/(b.second-a.second))/3.1415*180);
}
void handleEvent(int &upx , int &upy, std::pair<int,int> &mouse){
    SDL_Event event ;// đối tượng lưu các tương tác
    while(SDL_PollEvent(&event)!=0){
        switch(event.type){
                case SDL_QUIT:
                    // Nhận yêu cầu đóng cửa sổ
                    exit(0);
                    break;
                case SDL_KEYDOWN:
                    // xử lí sự kiện trên phím
                    switch(event.key.keysym.sym){

                        // Di chuyển lên W hoặc mũi tên lên
                        case SDLK_w:
                        case SDLK_UP:
                            upy++;
                            break;
                        // Di chuyển xuống S hoặc mũi tên xuống
                        case SDLK_s:
                        case SDLK_DOWN:
                            upy--;
                            break;
                         // Di chuyển sang trái A hoặc mũi tên trái
                         case SDLK_a:
                         case SDLK_LEFT:
                            upx--;
                            break;
                        // Di chuyển sang phải D hoặc mũi tên phải
                        case SDLK_RIGHT:
                        case SDLK_d:
                            upx++;
                            break;

                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    //Xử lý trường hợp là sự kiện nhấn chuột

                    // Kiểm tra xem có phải chuột trái không
                    if(event.button.button == SDL_BUTTON_LEFT){
                        mouse.first = event.button.x;
                        mouse.second = event.button.y;
                    }
        }
    }
}

// vd

void render(int r, int b, int g, int a, SDL_Rect rect, int angle, SDL_Renderer* renderer) {
    // Tạo một texture tạm với màu RGBA
    SDL_Surface* tempSurface = SDL_CreateRGBSurfaceWithFormat(0, rect.w, rect.h, 32, SDL_PIXELFORMAT_RGBA8888);
    if (!tempSurface) {
        SDL_Log("SDL_CreateRGBSurfaceWithFormat Error: %s", SDL_GetError());
        return;
    }
    // Tô màu cho surface
    SDL_FillRect(tempSurface, NULL, SDL_MapRGBA(tempSurface->format, r, g, b, a));

    // Chuyển surface thành texture
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    if (!tempTexture) {
        SDL_Log("SDL_CreateTextureFromSurface Error: %s", SDL_GetError());
        return;
    }

    // Xác định tâm xoay (center) của hình chữ nhật
    SDL_Point center = { rect.w / 2, rect.h / 2 };

    // Render texture với góc xoay được chỉ định
    SDL_RenderCopyEx(renderer, tempTexture, NULL, &rect, angle, &center, SDL_FLIP_NONE);

    // Hủy texture sau khi sử dụng
    SDL_DestroyTexture(tempTexture);
}



void act(SDL_Window *window , SDL_Renderer *renderer ){

    int window_w , window_h ;// kích thuuocws cửa sổ
    SDL_GetWindowSize(window,&window_w,&window_h); // hàm này yêu cầu đầu vào là con trỏ

    // Khởi tạo bitmap pixel trên màn hình
    pixelmp **mp = new pixelmp*[window_w+1];
    for(int i=0;i<=window_w;i++) mp[i]= new pixelmp[window_h+1];

    // Khởi tạo giá trị co bitmap pixel
    resetmp(window_w,window_h,mp);
    std::pair<int,int> mouse={0,0}; // chuột
    int upx, upy; // điều hướng upx, và di chuyển upy
    int rr1 =-90;// hướng của đối tượng
    int rr2 =0;// bước đi của đối tượng
    int rr3 =-90; // hướng súng của đối tượng
    SDL_Rect d1 ={300,300,200,200};//vị trí đối tượng
    SDL_Rect d2 ={350,350,100,100};// vị trí súng


    while(true){

        SDL_Delay(16);
        SDL_RenderClear(renderer);
        // đặt lại upx, upy;
        upy=0;
        upx=0;
        handleEvent(upx,upy,mouse);
        rr1+=upx*15;
        new_obj_location(d1,rr1,upy*10);//cập nhật vị trí đối tượng
        new_obj_location(d2,rr1,upy*10);// cập nhật vị trí súng
        int nr= degree(center_obj(d2),mouse);
        rr3=nr;
        render(77,77,77,77,d1,rr1,renderer);
        render(100,100,100,100,d2,-rr3,renderer);

        SDL_RenderPresent(renderer);

    }

}

/*
class pixelmp{
    public:
        int alpha ;// Do trong suot
        double speed ;// Hệ số làm chậm của môi trường
        int damage ;// Sat thuong tai pixel
        int attribute;// thuộc tính của đối tượng đang nắm giữ pixel
        void initialize(int al ,int sp, int dam,int att){
            alpha=al;
            speed=sp;
            damage =dam;
            attribute=att;
        }
};

struct TextureInfo{
    int root_id;// id gốc của đối tượng
    int id;//id của png được dùng hiện tại
    int scope;// phạm vi các khung ảnh của đối tượng
    int angle; // góc của hướng hiện tại đơn vị (độ);
    int speed ;// Tóc độ cơ bản của đối tượng
    double scale ; // Tỉ lệ tốc độ hiện tại
    int attribute; // thuộc tính của đối tượng
    int damage; // sát thương đối tượng
    int defense;// giáp của đối tượng
    SDL_Rect rect// vị trí và kích thước của đối tượng

};
void heart(int &cx, int &cy ,int x, int y, int w, int h);
double to_radian(int angle);
int to_degree(double angle);
void xoay_vector(int &ansX, int &ansY ,int x, int y, int cx, int cy , int angle );
void mapping(int &ansX, int &ansY , int cx, int cy , int px , int py , double scaleX, double scaleY);
void layer(bool ck ,SDL_Renderer *renderer, std::pair<SDL_Texture*, TextureInfo> *obj, pixelmp **mp);
*/




void autoact(SDL_Renderer *renderer ,SDL_Rect &mainrect,int upx,int upy ,TextureInfo &info, pixelmp **mp){

       SDL_Surface *surface = rbga(255,0,0,255);


        std::pair<int,int> obcent = center_obj(info.rect);
        std::pair<int,int> maincent=center_obj(mainrect);
        int dist= distance(obcent,maincent);
        int run = std::rand()%1;
        int pre_angle = info.angle;
        int pre_x =info.rect.x;
        int pre_y =info.rect.y;
        int step = std::rand()%10-5;
        if(info.attribute==1){
            /*if(info.scale==0.0 || dist>350){

                for(int i=1 ;i<=15;i++){
                    new_obj_location(info.rect,info.angle +i,step);
                    if(sol(surface,info.rect,mp)){
                            break;
                    }
                    new_obj_location(info.rect,info.angle -i ,step);
                    if(sol(surface,obj,mp)){
                            break;
                    }
                    info.rect.x=pre_x;
                    info.rect.y=pre_y;
                    info.angle =pre_angle;
                }

            }
            else{
                // Nếu không cách xa main và nằm trong khu vực thoáng -> đuổi theo main
                new_obj_location(info.rect,degree(std::make_pair(info.rect.x,info.rect.y),std::make_pair(mainrect.x,mainrect.y)),step);

            }
            */

        }
}
void preck(SDL_Window *window,SDL_Renderer *renderer){
    SDL_Rect mainrect ={400,400,400,400};
    SDL_Rect p1 ={200,200,50,100};
    SDL_Rect p2 ={600,600,50,50};

    int window_w , window_h ;// kích thuuocws cửa sổ
    SDL_GetWindowSize(window,&window_w,&window_h); // hàm này yêu cầu đầu vào là con trỏ

    // Khởi tạo bitmap pixel trên màn hình
    pixelmp **mp = new pixelmp*[window_w+1];
    for(int i=0;i<=window_w;i++) mp[i]= new pixelmp[window_h+1];

    // Khởi tạo giá trị co bitmap pixel
    resetmp(window_w,window_h,mp);

    TextureInfo t1,t2;

}
