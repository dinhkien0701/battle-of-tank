/* SDL_RenderPresent() ko bat buoc goi sau moi hanh dong ve mà co the khi xong khung hinh cung dc */
#include <SDL2/SDL.h>
#include <cstring>
#include <SDL2/SDL_Image.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include <algorithm>
#include <utility>
#include "menu.h"
#include "act.h"
#include "co_che.h"
#include "music.h"
using namespace std;

bool pick_area(SDL_Rect rect , pair<int,int> mouse , Sound &pick_mouse){
    if(rect.x<=mouse.first&&mouse.first<rect.x+rect.w){
        if(rect.y<=mouse.second && mouse.second < rect.y+rect.h){

            pick_mouse.play();// chọn chuột đúng vùng , phát nhạc
            return true ;
        }
    }
    return false ;
}

#include <algorithm> // cần dùng std::min

void menu_HUD(SDL_Renderer* renderer, TTF_Font* font, SDL_Rect rect, std::string text)
{
    // Vẽ nền của khung HUD
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Màu nền: xám đậm
    SDL_RenderFillRect(renderer, &rect);

    // Vẽ viền khung HUD
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Màu viền: trắng
    SDL_RenderDrawRect(renderer, &rect);

    // Màu chữ: trắng
    SDL_Color textColor = {255, 255, 255, 255};

    // Tạo surface từ chuỗi text bằng SDL_ttf
    SDL_Surface* surfaceText = TTF_RenderUTF8_Blended(font, text.c_str(), textColor);
    if (!surfaceText) {
        SDL_Log("Error creating text surface: %s", TTF_GetError());
        return;
    }

    // Tạo texture từ surface
    SDL_Texture* textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
    if (!textureText) {
        SDL_Log("Error creating texture: %s", SDL_GetError());
        SDL_FreeSurface(surfaceText);
        return;
    }

    // Lấy kích thước text gốc (đã được render với font kích thước ban đầu, giả sử là 32)
    int textWidth = surfaceText->w;
    int textHeight = surfaceText->h;

    // Xác định kích thước cho phép cho text:
    // - Chiều rộng cho phép là 80% của rect.w (để mỗi bên có lề 10%)
    // - Chiều cao không vượt quá rect.h
    int availableWidth = rect.w * 8 / 10;

    // Tính hệ số scale (mặc định là 1.0, tức không scale) nếu text vượt quá kích thước cho phép
    double scale = 1.0;
    if (textWidth > availableWidth || textHeight > rect.h) {
        double scaleW = static_cast<double>(availableWidth) / textWidth;
        double scaleH = static_cast<double>(rect.h) / textHeight;
        scale = std::min(scaleW, scaleH);
    }
    int finalWidth = static_cast<int>(textWidth * scale);
    int finalHeight = static_cast<int>(textHeight * scale);

    // Tính toán vị trí để căn giữa text trong khung HUD
    SDL_Rect destRect;
    destRect.w = finalWidth;
    destRect.h = finalHeight;
    destRect.x = rect.x + (rect.w - finalWidth) / 2;
    destRect.y = rect.y + (rect.h - finalHeight) / 2;

    // Render text đã scale vào renderer
    SDL_RenderCopy(renderer, textureText, NULL, &destRect);

    // Giải phóng tài nguyên
    SDL_FreeSurface(surfaceText);
    SDL_DestroyTexture(textureText);
}

void menustart(SDL_Window *window,SDL_Renderer *renderer , Sound &pick_mouse){
    TTF_Font* font = TTF_OpenFont("Font/OpenSans.ttf", 22);

    SDL_Surface* surface = IMG_Load("image/menu_background.png");//tai anh
    SDL_Surface* surfaceplay = IMG_Load("image/image_play.png");// tai anh play
    SDL_Surface* huong_dan = IMG_Load("image/guide.png");

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);// tao texture menu
    SDL_Texture* textureplay =SDL_CreateTextureFromSurface(renderer,surfaceplay);// tao texture paly
    SDL_Texture* Huong_dan =SDL_CreateTextureFromSurface(renderer,huong_dan);// tao texture  hình ảnh hướng dẫn

    SDL_FreeSurface(surface); // giai phong surface;
    SDL_FreeSurface(surfaceplay); // giai phong surfaceplay;
    SDL_FreeSurface(huong_dan);// giải phóng hình hướng dẫn

    //render nền

    int wpx,wpy;// khoi tao bien lay kich thuoc cua so
    SDL_GetWindowSize(window,&wpx,&wpy);// lay kich thuoc cua so
    int width , height ; // lay chieu rong , cao cua texture
    SDL_QueryTexture(texture,NULL,NULL,&width,&height);// lay kich thuoc texture ,khong quan tam loai pixel hay kieu truy cap( cung ko ro la gi )
    double scale = std::min(1.0d*wpx/width,1.0d*wpy/height); // ti le cua so so voi texture
    SDL_Rect dest_rect = {0.5*(wpx-scale*width),0.5*(wpy-scale*height),scale*width,scale*height}; // vi tri va kich thuoc in hinh - (in toan man cua so)
    SDL_RenderCopy(renderer,texture,NULL,&dest_rect);// NULL: in toan bo texture ma khong cat phan nao

    // rect của núi point và guide dựa theo hình nền

    SDL_Rect rect_point = {0.15*dest_rect.w+dest_rect.x,0.8*dest_rect.h+dest_rect.y,0.25*dest_rect.w,0.07*dest_rect.h};
    SDL_Rect rect_guide = {0.6*dest_rect.w+dest_rect.x,0.8*dest_rect.h+dest_rect.y,0.25*dest_rect.w,0.07*dest_rect.h};





    //render nút play , tạo bộ đếm elsapsed ( fps )

    int x,y ; // chieu rong và cao cua texturepalay
    SDL_QueryTexture(textureplay, NULL, NULL, &x, &y);// lay chieu rong va chieu cao cho x,y;
    SDL_Event event;
    while(SDL_PollEvent(&event));// xoa het cac thao tac phim thua
    double elapsed = 0 ;// khoi dong bo dem
    int x1,y1; // chieu rong va cao theo thoi gian bo dem elapsedtime;
    int x2,y2;// vi tri in textureplay
    SDL_Rect dest_rect_play ={0,0,0,0}; // khoi tao dest_rect textureplay

    //Hệ thống biến để sywr dụng hàm handle
    pair<int,int> mouse;
    int upx,upy; // biến cho có
    bool t1 ,t2 ,t3 ; // biến cho có

    while(true){
        // tạo sự chuyển động lớn nhỏ cho nút play
        x1= x-abs(sin(elapsed)*x/4);
        y1= y-abs(sin(elapsed)*y/4);
        elapsed+=0.02d;
        x2= 0.5d*(wpx-x1);//vi tri x2 khi in texture
        y2= 0.45d*wpy-0.5d*y1; // vi tri y2 khi in textureplay
        dest_rect_play ={x2,y2,x1,y1};

        SDL_RenderCopy(renderer,texture,NULL,&dest_rect);
        SDL_RenderCopy(renderer,textureplay,NULL,&dest_rect_play);
        menu_HUD(renderer, font, rect_point, "ĐIỂM CAO");
        menu_HUD(renderer, font, rect_guide, "HƯỚNG DẪN");
        SDL_RenderPresent(renderer);// hien thi noi dung sau khi da hoan thành cho khung hinh

        //kiểm tra thao tác
        handleEvent(upx,upy,mouse,t1,t2,t3);

        if(pick_area(dest_rect_play,mouse,pick_mouse)){
            // bắt đầu vào màn chơi
            return ;
        }

        else if(pick_area(rect_point,mouse,pick_mouse)){
            //nếu người chơi muốn xem điểm
            SDL_Rect rect_back = {0.1*dest_rect.w+dest_rect.x,0.6*dest_rect.h+dest_rect.y,0.30*dest_rect.w,0.07*dest_rect.h};
            SDL_Rect rect_delete = {0.5*dest_rect.w+dest_rect.x,0.6*dest_rect.h+dest_rect.y,0.40*dest_rect.w,0.07*dest_rect.h};
            SDL_Rect history_point = {0.2*dest_rect.w+dest_rect.x,0.4*dest_rect.h+dest_rect.y,0.6*dest_rect.w,0.1*dest_rect.h};
            int highestpoint = read_high_point();
            do{
                 SDL_RenderClear(renderer);

                 SDL_RenderCopy(renderer,texture,NULL,&dest_rect);
                 menu_HUD(renderer, font, history_point, "ĐIỂM CAO NHẤT ĐẠI ĐƯỢC : " + to_string(highestpoint) );
                 menu_HUD(renderer, font, rect_back, "QUAY LẠI");
                 menu_HUD(renderer, font, rect_delete, "ĐẶT LẠI ĐIỂM CAO NHẤT");

                 SDL_RenderPresent(renderer);
                 handleEvent(upx,upy,mouse,t1,t2,t3);
                 SDL_Delay(16);
                 if(pick_area(rect_delete,mouse,pick_mouse)==true){
                        write_point(0);
                        highestpoint = 0;
                 }

            }while(pick_area(rect_back,mouse,pick_mouse)==false);
        }
        else if(pick_area(rect_guide,mouse,pick_mouse)){
           SDL_Rect rect_text = {dest_rect.x,dest_rect.y+ 0.1*dest_rect.h,dest_rect.w,0.65*dest_rect.h};
           SDL_Rect rect_back = {dest_rect.x,dest_rect.y+0.85*dest_rect.h,dest_rect.w,0.1*dest_rect.h};
           do{
                 SDL_RenderClear(renderer);

                 SDL_RenderCopy(renderer,texture,NULL,&dest_rect);
                 SDL_RenderCopy(renderer, Huong_dan ,NULL, &rect_text);
                 menu_HUD(renderer, font, rect_back, "QUAY LẠI");

                 SDL_RenderPresent(renderer);
                 handleEvent(upx,upy,mouse,t1,t2,t3);
                 SDL_Delay(16);

            }while(pick_area(rect_back,mouse,pick_mouse)==false);
        }
        SDL_Delay(16);
        SDL_RenderClear(renderer);
    }

}
