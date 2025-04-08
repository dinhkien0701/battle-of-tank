#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <utility>
#include <algorithm>
#include "loading.h"
#include "menu.h"
#include "co_che.h"
#include "act.h"
/*
void rap_level(std::vector<SDL_Rect> &vRect) {
    // logic xử lý
}
bool kiem_tra_vung_chuot (const SDL_Rect &rect , std::pair<int,int> mouse){
    //
    if(mouse.first<rect.x||mouse.first>rect.x+rect.w)return false;
    if(mouse.second <rect.y||mouse.second>rect.y+rect.h)return false;
    return true;
}

int level_menu(SDL_Window *window , SDL_Renderer *renderer){
    int upx ,upy;
    std::pair<int,int> mouse_left , mouse_right;
    std::vector<SDL_Rect> vRect;
    while( true ){
        upx=upy =0; // hiện tại chưa dùng nên đưa về 0 để tránh tràn số
        mouse_left =mouse_right = std::make_pair(-1,-1); // reset lại chuột
        bool k;
        handleEvent(upx,upy,mouse_left,mouse_right,k);
        for(int i=0;i<10;i++){
            if(kiem_tra_vung_chuot(vRect[i],mouse_left)){
                return i+1 ; // Đưa ra level người chơi lựa chọn
            }
        }
    }
}

void history_point(int level){
    //Tải lịch sử điểm và hiển thị bắt đầu
}
*/
