#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <utility>
#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <SDL2/SDL_ttf.h>
#include "co_che.h"
#include "act.h"

using namespace std;

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
            mp[i][j].initialize(0,0);
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
void handleEvent(int &upx , int &upy, std::pair<int,int> &mouse_left , bool & ban_dan){
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
                        case SDLK_SPACE :
                            ban_dan = true ;
                            break;

                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    //Xử lý trường hợp là sự kiện nhấn chuột

                    // Kiểm tra xem có phải chuột trái không
                    if(event.button.button == SDL_BUTTON_LEFT){
                        mouse_left.first = event.button.x;
                        mouse_left.second = event.button.y;
                    }

        }
    }
}

// vd

void render(SDL_Rect rect, int angle, SDL_Renderer* renderer ,SDL_Surface *tempSurface) {

    // Chuyển surface thành texture
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
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




bool auto_act(SDL_Surface *surface , TextureInfo &main ,TextureInfo &enemy , pixelmp **mp){
    bool ban_dan =false ; // có nên bắn đạn không

    //Lấy tâm của enemy {x,y} và tâm main {cx,cy}
    int x,y,cx,cy;

    heart(x,y,enemy.rect.x,enemy.rect.y,enemy.rect.w,enemy.rect.h); // lấy tâm enemy;
    heart(cx,cy,main.rect.x,main.rect.y,main.rect.w,main.rect.h); // lấy tâm main
    /*cout<<x<<' '<<y<<' '<<cx<<' '<<cy<<'\n';
    cx=cy =0; */
    // Tính khoảng cách từ địch đến nhân vật
    int dist = distance(x,y,cx,cy);
    cout<<dist<<' ';
    SDL_Rect new_rect ; // biến tạo trước để xác định vị trí

    if(dist <=200){

        int degree = (static_cast<int>(180*atan2(1.0*(cy-y),1.0*(cx-x))/3.1415)+360)%360;
        degree =(degree+360)%360; // chuẩn hóa về thang 360 độ
        int rand_degree = 3 +rand()%1 ; // hướng của súng sẽ quy một số độ
        if((180<degree-enemy.angle_two &&degree-enemy.angle_two<360)||(-180<degree-enemy.angle_two &&degree-enemy.angle_two <0)){
            //Xác định hướng xoay tối ưu ;
            enemy.angle_two =(enemy.angle_two - rand_degree +360)%360;
        }
        else enemy.angle_two =(enemy.angle_two +rand_degree +360)%360;

        //cout<< degree<<' ';
        if(enemy.adjec[3]==1){
            ban_dan=true;
        }
        else if(enemy.adjec[3]==2){

            int do_xoay =10 ; // độ xoay tối đa khi kiểm tra
            do_xoay =min(do_xoay,abs(degree-enemy.angle));
            do_xoay =min(do_xoay,min(abs(enemy.angle+360- degree),(degree +360 -enemy.angle)));
            int huong =1;
            if((180<degree-enemy.angle &&degree-enemy.angle<360)||(-180<degree-enemy.angle &&degree-enemy.angle <0)){
                huong =-1;
            }
            for(int i=do_xoay ;i>0;i--){
                new_rect =enemy.rect;

                if(sol(surface,new_rect,enemy.id,(enemy.angle+ huong*i +360)%360,mp)){
                    enemy.angle = (enemy.angle+ huong*i +360)%360;
                    break;
                }
            }
        }
        else if(enemy.adjec[3]==4){

            for(int i=6;i>0;i--){
                new_rect =enemy.rect;
                new_obj_location(new_rect,enemy.angle,i);
                if(sol(surface,new_rect,enemy.id,enemy.angle,mp)){
                    enemy.rect =new_rect;
                    break;
                }
            }
        }
        enemy.adjec[3]=(enemy.adjec[3]+1)%5;
        return ban_dan;

    }
    if(/*enemy.adjec[0]+enemy.adjec[1]+enemy.adjec[2]==60*/ true){
        cout<<"pp";
        enemy.adjec[0]=enemy.adjec[1]=enemy.adjec[2]=0 ; // sau khi xong một chu kỳ thì reset

        for(int i=1;i<=90;i++){
            for(int j=15;j>0;j--){
               new_rect =enemy.rect;
               new_obj_location(new_rect,enemy.angle+ i,j);
               if(sol(surface,new_rect,enemy.id,enemy.angle+i,mp)){
                    enemy.rect =new_rect;
                    enemy.angle  =(enemy.angle +i+360)%360;
               }
               new_rect =enemy.rect;
               new_obj_location(new_rect,enemy.angle- i,j);
               if(sol(surface,new_rect,enemy.id,enemy.angle-i,mp)){
                    enemy.rect= new_rect;
                    enemy.angle  =(enemy.angle -i+360)%360;
               }


            }
        }

    }
    else{
        cout<<"ss";
        int active =rand()%3;
        while(enemy.maxadjec[active]==enemy.adjec[active]){
            active =rand()%3;
            }
        enemy.adjec[active]++;

        if(active ==1){
            // lựa chọn xoay
            for(int i=10;i>0;i++){
                new_rect = enemy.rect;
                if(sol(surface,new_rect,enemy.id,(enemy.angle+i +360)%360,mp)){
                    enemy.angle =(enemy.angle+i +360)%360;
                }
                new_rect = enemy.rect;
                if(sol(surface,new_rect,enemy.id,(enemy.angle-i +360)%360,mp)){
                   enemy.angle =(enemy.angle-i +360)%360;
                }
            }
        }
        else{
            // lựa chọn tiến lùi
            for(int i=6;i>i;i--){
                new_rect = enemy.rect;
                new_obj_location(new_rect,enemy.angle,i);
                if(sol(surface,new_rect,enemy.id,enemy.angle,mp)){
                    enemy.rect =new_rect;
                }
                new_rect = enemy.rect;
                new_obj_location(new_rect,enemy.angle,-i);
                if(sol(surface,new_rect,enemy.id,enemy.angle,mp)){
                    enemy.rect =new_rect;
                }
            }
        }
        enemy.angle_two =(enemy.angle_two + rand()%2 +360 - rand()%2)%360;

    }

    return ban_dan;
}
bool dan(SDL_Surface *surface , TextureInfo &dan ,pixelmp **mp){
    bool trung_muc_tieu =false ; // liệu đạn có trúng mục tiêu không

    new_obj_location(dan.rect,dan.angle,3);

    Uint32 *pixels =(Uint32*)surface->pixels; // tạo con trỏ đến bảng màu của ảnh
    int pitch = surface->pitch / sizeof(Uint32);  // Số pixel trên mỗi dòng (có padding)

    int width = surface->w; //chiều rộng ảnh
    int height = surface->h; // chiều cao ảnh

    int cx,cy; // tâm của ảnh png
    int px, py;// tâm của đối tượng trên khung hình
    heart(cx,cy,0,0,width,height); // tinh cx,cy
    heart(px,py,dan.rect.x,dan.rect.y,dan.rect.w,dan.rect.h); // tính px, py
    int ansX, ansY ;// biến dể lưu điểm ảnh qua ánh xạ kép xoay_vector và mapping
    Uint8 r,b,g,a ;// màu và độ trong
   // Duyệt qua từng pixel trong ảnh png
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
                Uint32 pixel = pixels[j*pitch+i]; // tính chỉ số của pixel ( mảng hai chiều của pixels được đưa vè một chiều )
                SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a); // lấy thông tin pixel ảnh

                // Ánh xạ ansX, ansY hai lần để có được vị trí hiện thị của điểm ảnh lên màn hình
                xoay_vector(ansX,ansY,i,j,cx,cy,dan.angle);
                mapping(ansX,ansY,cx,cy,px,py,1.0*dan.rect.w/width,1.0*dan.rect.h/height);
                if(a>0&&(ansX<0||ansX>1319)&&(ansY<40||ansY>759))return false;
                if(a>0&&mp[ansX][ansY].attribute&&dan.id!=mp[ansX][ansY].attribute){
                    //Tồn tại tiếp xúc giữa vật thể với môi trường
                    trung_muc_tieu = true;
                }
        }
    }
    return trung_muc_tieu ;
}

void lua_chon(SDL_Renderer *renderer , SDL_Surface *surface ,TextureInfo &player,int &upx , int &upy, pixelmp **mp ){
    int new_angle = player.angle;
    int new_angle_two = player.angle_two;
    SDL_Rect rect = player.rect;
    // xác định góc lựa chọn tăng hay giảm
    if(upx > 0){
        new_angle =(new_angle + 3 +360)%360 ;
    }
    else if(upx <0){
        new_angle =(new_angle -3+ +360)%360;
    }
    //xác định tiến hay lùi
    if(upy > 0)upy =3;
    else if(upy <0)upy =-3;
    new_obj_location(rect,new_angle,upy);

    if(sol(surface,rect,player.id,new_angle,mp)){
        // nếu vùng này đến được , xóa vị trí trước
        for(int i = player.rect.x -10 ; i<= player.rect.x + player.rect.w+10  ; i++ ){
            for(int j = player.rect.y-10; j<=player.rect.y +player.rect.h +10 ; j++){
                if(i<0||i>1319||j<40||j>759)continue;
                if(mp[i][j].alpha== player.id){
                    mp[i][j].initialize(0,0);
                }
             }
        }
        // cập nhật vị trí mới
        player.rect =rect;
        player.angle =new_angle;
        layer(surface , player , mp);
        upx = upy = 0;

    }
    SDL_Point center = {rect.w / 2 , rect.h / 2};
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_RenderCopyEx(renderer,texture,NULL,&player.rect,player.angle,&center,SDL_FLIP_NONE);
    SDL_DestroyTexture(texture);

}


// Hàm này vẽ khung HUD ở đầu màn hình với các thông tin: Level, Point, Highest point, Hearts.
void drawHUD(SDL_Renderer* renderer, TTF_Font* font, int level, int points, int highestPoint, int hearts)
{
    // Vẽ nền của khung HUD
    SDL_Rect hudRect = {0, 0, 1320, 39};
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Màu nền: xám đậm
    SDL_RenderFillRect(renderer, &hudRect);

    // Vẽ viền khung HUD
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Màu viền: trắng
    SDL_RenderDrawRect(renderer, &hudRect);

    // Tạo các chuỗi hiển thị
    std::string levelText       = "Level: " + std::to_string(level);
    std::string pointsText      = "Point: " + std::to_string(points);
    std::string highestText     = "Highest point: " + std::to_string(highestPoint);
    std::string heartsText      = "Hearts: " + std::to_string(hearts);

    // Màu chữ
    SDL_Color textColor = {255, 255, 255, 255};

    // Tạo surface từ các chuỗi bằng SDL_ttf (có thể dùng TTF_RenderText_Blended cho chất lượng tốt)
    SDL_Surface* surfaceLevel   = TTF_RenderText_Blended(font, levelText.c_str(), textColor);
    SDL_Surface* surfacePoints  = TTF_RenderText_Blended(font, pointsText.c_str(), textColor);
    SDL_Surface* surfaceHighest = TTF_RenderText_Blended(font, highestText.c_str(), textColor);
    SDL_Surface* surfaceHearts  = TTF_RenderText_Blended(font, heartsText.c_str(), textColor);

    if (!surfaceLevel || !surfacePoints || !surfaceHighest || !surfaceHearts) {
        SDL_Log("Error creating text surface: %s", TTF_GetError());
        return;
    }

    // Tạo texture từ surface
    SDL_Texture* textureLevel   = SDL_CreateTextureFromSurface(renderer, surfaceLevel);
    SDL_Texture* texturePoints  = SDL_CreateTextureFromSurface(renderer, surfacePoints);
    SDL_Texture* textureHighest = SDL_CreateTextureFromSurface(renderer, surfaceHighest);
    SDL_Texture* textureHearts  = SDL_CreateTextureFromSurface(renderer, surfaceHearts);

    if (!textureLevel || !texturePoints || !textureHighest || !textureHearts) {
        SDL_Log("Error creating texture: %s", SDL_GetError());
        // Giải phóng surface trước khi return
        SDL_FreeSurface(surfaceLevel);
        SDL_FreeSurface(surfacePoints);
        SDL_FreeSurface(surfaceHighest);
        SDL_FreeSurface(surfaceHearts);
        return;
    }

    // Tính toán vị trí cho mỗi chuỗi trong khung HUD
    SDL_Rect destRect;

    // Vẽ "Level: ..." tại vị trí x = 20
    destRect.x = 20;
    destRect.y = (40 - surfaceLevel->h) / 2;
    destRect.w = surfaceLevel->w;
    destRect.h = surfaceLevel->h;
    SDL_RenderCopy(renderer, textureLevel, NULL, &destRect);

    // Vẽ "Point: ..." tại vị trí x = 400 (có thể điều chỉnh theo giao diện)
    destRect.x = 400;
    destRect.y = (40 - surfacePoints->h) / 2;
    destRect.w = surfacePoints->w;
    destRect.h = surfacePoints->h;
    SDL_RenderCopy(renderer, texturePoints, NULL, &destRect);

    // Vẽ "Highest point: ..." tại vị trí x = 800
    destRect.x = 800;
    destRect.y = (40 - surfaceHighest->h) / 2;
    destRect.w = surfaceHighest->w;
    destRect.h = surfaceHighest->h;
    SDL_RenderCopy(renderer, textureHighest, NULL, &destRect);

    // Vẽ "Hearts: ..." tại vị trí x = 1200
    destRect.x = 1200;
    destRect.y = (40 - surfaceHearts->h) / 2;
    destRect.w = surfaceHearts->w;
    destRect.h = surfaceHearts->h;
    SDL_RenderCopy(renderer, textureHearts, NULL, &destRect);

    // Giải phóng bộ nhớ của surfaces và textures
    SDL_FreeSurface(surfaceLevel);
    SDL_FreeSurface(surfacePoints);
    SDL_FreeSurface(surfaceHighest);
    SDL_FreeSurface(surfaceHearts);

    SDL_DestroyTexture(textureLevel);
    SDL_DestroyTexture(texturePoints);
    SDL_DestroyTexture(textureHighest);
    SDL_DestroyTexture(textureHearts);
}
