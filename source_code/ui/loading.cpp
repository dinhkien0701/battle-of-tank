#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "loading.h"
void loadingstart(SDL_Window *window,SDL_Renderer *renderer){
    SDL_Surface* surface = IMG_Load("C:/Users/maidi/Downloads/ld.png");//tai anh
    if(!surface){
        SDL_Log("Khong tai dc anh loading %s",IMG_GetError());
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    if(!texture){
        SDL_Log("Tao texture loading start that bai%s",SDL_GetError());
        return;
    }
    SDL_FreeSurface(surface); // giai phong surface;
    int width , height ; // lay chieu rong , cao cua texture
    SDL_QueryTexture(texture,NULL,NULL,&width,&height);// khong quan tam loai pixel hay kieu truy cap( cung ko ro la gi )

    int x , y; // kich thuoc cua so lam viec
    SDL_GetWindowSize(window,&x,&y);// ham lay kich thuoc
    double scale = std::min((double)x/width,(double)y/height);// scale : ti le giua texture và cua so
    SDL_Rect dest_rect = {0.5*((double)x-scale*width),0.5*((double)y-scale*height),scale*width,scale*height}; // vi tri va kich thuoc in hinh - (in toan man cua so)
    SDL_RenderCopy(renderer,texture,NULL,&dest_rect);// NULL: in toan bo texture ma khong cat phan nao
    SDL_RenderPresent(renderer); // Dua noi dung da ve ra man hinh
     // RenderCopy se tu dong keo dan texture cho vua man hinh

    // Tao vung cat cho chu loading ow duoi mau trang

    // Hieu ung lap lanh trong 3,5s

    int totaltime =3500 ; // 3500 mili giay
    int elapsedtime =0; // thoi gian thanh chay
    int progresbar =0; // kich thuoc thanh mau trang den hien tai

    while(elapsedtime < totaltime){
        progresbar = x*elapsedtime/totaltime; // chieu rong thanh mau trang den hien tai
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // ve thanh mau trang ben trong khung
         SDL_Rect whitebar ={0,15*y/16,progresbar,y-15*y/16};// kich thuoc khung de de len
         SDL_RenderFillRect(renderer,&whitebar); // luu noi dung vao renderer(but ve )
         SDL_RenderPresent(renderer); // Hien thi noi dung khung hinh da hoan thanh

         // Lam muot bang 60 fps
         SDL_Delay(16); // delay 16ms
         elapsedtime+=16;

         // Lam moi anh nen

         SDL_RenderClear(renderer);// xoa toan bo noi dung tren cua so
         SDL_RenderCopy(renderer,texture,NULL,&dest_rect);

    }
    SDL_DestroyTexture(texture);
    SDL_RenderClear(renderer);
}
