#ifndef ACT_H
#define ACT_H

#include<SDL2/SDL_ttf.h>

void resetmp(int w, int h , pixelmp **mp);

void new_obj_location(SDL_Rect &rect , int angle ,int step);

std::pair<int,int> center_obj(const SDL_Rect &rect);

int degree(std::pair<int,int> a, std::pair<int,int> b);

void handleEvent(int &upx , int &upy, std::pair<int,int> &mouse , bool & ban_dan);

void render(SDL_Rect rect, int angle, SDL_Renderer* renderer,SDL_Surface *tempSurface) ;

void act(SDL_Window *window , SDL_Renderer *renderer );


struct typemap{
    //Loại bản đồ người chơi sẽ gặp
    int id;
    int scaleW;
    int scaleH;
    void initialize(int ID, int SCALE_W, int SCALE_H){
        id=ID;
        scaleH=SCALE_H;
        scaleW=SCALE_W;
    }
};

 //SDL_Rect autoact(SDL_Renderer *renderer ,SDL_Rect &rest,SDL_Rect &mainrect,int upx,int upy , std::pair<SDL_Texture*, TextureInfo> *obj, pixelmp **mp);

void handleEvent(int &upx , int &upy, std::pair<int,int> &mouse_left , std::pair<int,int> &mouse_right , bool & ban_dan);

void drawHUD(SDL_Renderer* renderer, TTF_Font* font, int level, int points, int highestPoint, int hearts);
void lua_chon(SDL_Renderer *renderer , SDL_Surface *surface ,TextureInfo &player,int &upx , int &upy, pixelmp **mp );
bool auto_act(SDL_Surface *surface , TextureInfo &main ,TextureInfo &enemy , pixelmp **mp);

#endif // ACT_H
