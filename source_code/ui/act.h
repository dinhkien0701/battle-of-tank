#ifndef ACT_H
#define ACT_H

void resetmp(int w, int h , pixelmp **mp);

void new_obj_location(SDL_Rect &rect , int angle ,int step);

std::pair<int,int> center_obj(const SDL_Rect &rect);

int degree(std::pair<int,int> a, std::pair<int,int> b);

void handleEvent(int &upx , int &upy, std::pair<int,int> &mouse);

void render(int r, int b, int g, int a, SDL_Rect rect, int angle, SDL_Renderer* renderer) ;

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

void autoact(SDL_Renderer *renderer ,SDL_Rect &rest,SDL_Rect &mainrect,int upx,int upy , std::pair<SDL_Texture*, TextureInfo> *obj, pixelmp **mp);

#endif // ACT_H
