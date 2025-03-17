/* SDL_RenderPresent() ko bat buoc goi sau moi hanh dong ve mà co the khi xong khung hinh cung dc */
#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <cmath>
#include <algorithm>
#include "menu.h"
bool menustart(SDL_Window *window,SDL_Renderer *renderer){
    SDL_Surface* surface = IMG_Load("C:/Users/maidi/Downloads/st.png");//tai anh
    if(!surface){
        SDL_Log("Khong tai dc anh menu %s",IMG_GetError());
        return false;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);// tao texture menu
    if(!texture){
        SDL_Log("Tao texture menu that bai%s",SDL_GetError());
        return false;
    }
    SDL_FreeSurface(surface); // giai phong surface;
    int wpx,wpy;// khoi tao bien lay kich thuoc cua so
    SDL_GetWindowSize(window,&wpx,&wpy);// lay kich thuoc cua so
    int width , height ; // lay chieu rong , cao cua texture
    SDL_QueryTexture(texture,NULL,NULL,&width,&height);// lay kich thuoc texture ,khong quan tam loai pixel hay kieu truy cap( cung ko ro la gi )
    double scale = std::min(1.0d*wpx/width,1.0d*wpy/height); // ti le cua so so voi texture
    SDL_Rect dest_rect = {0.5*(wpx-scale*width),0.5*(wpy-scale*height),scale*width,scale*height}; // vi tri va kich thuoc in hinh - (in toan man cua so)
    SDL_RenderCopy(renderer,texture,NULL,&dest_rect);// NULL: in toan bo texture ma khong cat phan nao



    SDL_Surface* surfaceplay = IMG_Load("C:/Users/maidi/Downloads/images.png");// tai anh play
    if(!surfaceplay){
        SDL_DestroyTexture(texture);// neu loi hay xoa het bo nho da chiem dung tranh ro ri
        SDL_Log("Khong tai dc anh play %s",IMG_GetError());
        return false;
    }
    SDL_Texture* textureplay =SDL_CreateTextureFromSurface(renderer,surfaceplay);// tao texture paly
    if(!textureplay){
        SDL_DestroyTexture(texture);// neu loi hay xoa het bo nho da chiem dung tranh ro ri
        SDL_Log("Tao texture play that bai%s",SDL_GetError());
        return false;
    }
    SDL_FreeSurface(surfaceplay);// giai phong surfaceplay

    int x,y ; // chieu rong và cao cua texturepalay
    SDL_QueryTexture(textureplay, NULL, NULL, &x, &y);// lay chieu rong va chieu cao cho x,y;
    SDL_Event event;
    while(SDL_PollEvent(&event));// xoa het cac thao tac phim thua
    double elapsed = 0 ;// khoi dong bo dem
    int x1,y1; // chieu rong va cao theo thoi gian bo dem elapsedtime;
    int x2,y2;// vi tri in textureplay
    SDL_Rect dest_rect_play ={0,0,0,0}; // khoi tao dest_rect textureplay
    while(true){
        x1= x-abs(sin(elapsed)*x/4);
        y1= y-abs(sin(elapsed)*y/4);
        elapsed+=0.02d;
        x2= 0.5d*(wpx-x1);//vi tri x2 khi in texture
        y2= 0.45d*wpy-0.5d*y1; // vi tri y2 khi in textureplay
        dest_rect_play ={x2,y2,x1,y1};
        SDL_RenderCopy(renderer,textureplay,NULL,&dest_rect_play);
        SDL_RenderPresent(renderer);// hien thi noi dung sau khi da hoan thành cho khung hinh
        if(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT){
                // sdl_quit : thao tac do la dong cua so
                // don dep va return
                SDL_RenderClear(renderer);
                SDL_DestroyTexture(texture);
                SDL_DestroyTexture(textureplay);
                return false;
            }
            else if(event.type ==SDL_MOUSEBUTTONDOWN){
                // Neu la thao tac Bam chuot
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                if(mouseX>=x2&&mouseX<=x1+x2&&mouseY>=y2&&mouseY<=y1+y2){
                    // don dep va return
                    SDL_RenderClear(renderer);
                    SDL_DestroyTexture(texture);
                    SDL_DestroyTexture(textureplay);
                    return true ;// neu bam chuot vao play , vao man hinh choi game
                }
            }
        }
        SDL_Delay(16);
        SDL_RenderClear(renderer);// xoa toan bo noi dung tren cua so
        SDL_RenderCopy(renderer,texture,NULL,&dest_rect);//tao lap lai cua so
        if(elapsed >3.13d)elapsed=0.0d;
    }
    return false ; // return cho du thanh phan thoi

}
