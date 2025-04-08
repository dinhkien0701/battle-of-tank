#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <utility>
#include <algorithm>
#include <list>
#include <cstdlib>
#include <ctime>
#include "loading.h"
#include "menu.h"
#include "co_che.h"
#include "act.h"
using namespace std;
SDL_Surface *nen_surface = IMG_Load("C:/Users/maidi/Downloads/z6454705968533_fb6131d252bf4e6c8d54e5c856179c33.jpg");
    // Main
SDL_Surface *xe = IMG_Load("C:/Users/maidi/Downloads/mann.png");

    //Đich
SDL_Surface *than_dich = IMG_Load("C:/Users/maidi/Downloads/than_dich_chuan.png");
SDL_Surface *sung_dich = IMG_Load("C:/Users/maidi/Downloads/than_sung_dich.png");

    //Tường thành

SDL_Surface *tuong = IMG_Load("C:/Users/maidi/Downloads/z6455392080920_50cdaad3f6b5dc71701d80f8dbee46e5.png");

SDL_Surface *dan_surface = IMG_Load("C:/Users/maidi/Downloads/vien_dannnn.png");


void khoi_tao_vien(const SDL_Rect &rect , const SDL_Rect &pre_rect ,const int &id ,const int &attribute,pixelmp **mp){
    // xóa viền ở vị trí cũ
    for(int i=0;i<pre_rect.w;i++){
        mp[pre_rect.x+i][pre_rect.y].initialize(0,0);
        mp[pre_rect.x+i][pre_rect.y+pre_rect.h-1].initialize(0,0);
    }
    for(int i=0;i<pre_rect.h;i++){
        mp[pre_rect.x][pre_rect.y+i].initialize(0,0);
        mp[pre_rect.x+pre_rect.w-1][pre_rect.y+i].initialize(0,0);
    }
    // vẽ viền ở vị trí mới
    for(int i=0;i<rect.w;i++){
        mp[rect.x+i][rect.y].initialize(id,attribute);
        mp[rect.x+i][rect.y+rect.h-1].initialize(id,attribute);
    }
    for(int i=0;i<pre_rect.h;i++){
        mp[pre_rect.x][pre_rect.y+i].initialize(id,attribute);
        mp[pre_rect.x+pre_rect.w-1][pre_rect.y+i].initialize(id,attribute);
    }

}
void xoa_khoi(TextureInfo &obb, SDL_Rect pre_rect ,pixelmp **mp){
    for(int i = pre_rect.x -15 ; i<= pre_rect.x + pre_rect.w+15  ; i++ ){
        for(int j = pre_rect.y-15; j<=pre_rect.y +pre_rect.h +15 ; j++){
            if(i<0||i>1319||j<40||j>759)continue;
            if(mp[i][j].alpha == obb.id){
                mp[i][j].initialize(0,0);
            }
        }
    }
}
void print_enemy( SDL_Renderer *renderer ,TextureInfo &enemy , SDL_Rect pre_rect, pixelmp **mp){
    // xóa khỏi vj trí cũ và đặt vị trí mới cho viền xe địch

    mapping_dich(than_dich,enemy,mp); // điền lại vị trí mới vào bảng
    SDL_Texture* texture_than = SDL_CreateTextureFromSurface(renderer,than_dich);
    SDL_Texture* texture_sung = SDL_CreateTextureFromSurface(renderer,sung_dich);
    SDL_Point center = { enemy.rect.w / 2, enemy.rect.h / 2 };
    SDL_RenderCopyEx(renderer, texture_than, NULL, &enemy.rect, enemy.angle, &center, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, texture_sung, NULL, &enemy.rect, enemy.angle_two, &center, SDL_FLIP_NONE);
}
void print_tuong(SDL_Renderer *renderer , TextureInfo &tuong_chan , pixelmp **mp){
    SDL_Texture* texture_tuong = SDL_CreateTextureFromSurface(renderer,tuong);
    SDL_RenderCopy(renderer,texture_tuong,NULL,&tuong_chan.rect);
}
void print_main( SDL_Renderer *renderer ,TextureInfo &player , SDL_Rect pre_rect, pixelmp **mp){
    /* xóa khỏi vj trí cũ
    xoa_khoi(player,pre_rect,mp); */

    SDL_Texture* texture_main = SDL_CreateTextureFromSurface(renderer,xe);
    SDL_Point center = { player.rect.w / 2,player.rect.h / 2 };
    SDL_RenderCopyEx(renderer, texture_main, NULL, &player.rect, player.angle, &center, SDL_FLIP_NONE);


}
void run_game(SDL_Window *window , SDL_Renderer *renderer){
    TTF_Font* font = TTF_OpenFont("C:/SDL2_ttf-2.24.0/OpenSans-Italic-VariableFont_wdth,wght.ttf", 24);
    int window_w , window_h;
    SDL_SetWindowSize(window,1320,760); // thay đổi kích thước của sổ

    // Căn giữa cửa sổ cho phù hợp
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    SDL_GetWindowSize(window , &window_w, &window_h);
    // Khởi tạo bitmap pixel trên màn hình
    pixelmp **mp = new pixelmp*[window_w+300];
    for(int i=0;i<=window_w;i++) mp[i]= new pixelmp[window_h+500];

    // Khởi tạo giá trị co bitmap pixel
    resetmp(window_w,window_h,mp);

    int level =10;
    int point =0;
    int highestpoint=0;
    int pre_dich = 5 ; // số địch trước khi cập nhật
    TextureInfo *enemy_list = new TextureInfo[22];
    TextureInfo *obj_list   = new TextureInfo[400];
    TextureInfo player;
    int so_luong_dich, so_vat_can ;
    bool ban_dan =false ;
    int fps_ban_dan = 25 ;
    player.defense = 3; // người chơi ban đầu có 3 mạng

    while(player.defense>0){

        SDL_RenderClear(renderer); // làm sạch bút vẽ

        // Khởi tạo hạt giống cho hàm rand để sinh ngẫu nhiên
        srand(std::time(0));


        map_khoi_dong( level , enemy_list ,obj_list , player ,so_luong_dich , so_vat_can);

        print_main(renderer,player,player.rect,mp);
        for(int i=10;i<so_luong_dich;i++){
            print_enemy(renderer,enemy_list[i],enemy_list[i].rect,mp);

        }
        for (int i=20;i<so_vat_can;i++){
            print_tuong(renderer, obj_list[i],mp);

            //vẽ viền cho tường

            khoi_tao_vien(obj_list[i].rect,obj_list[i].rect,i,3,mp);

        }



        //cout<<player.rect.x<<" "<<player.rect.y<<" "<<player.rect.w<<" "<<player.rect.h;
        SDL_RenderPresent(renderer);

        SDL_Delay(3000);

        pair<int,int> mouse;
        int upx=0,upy=0;
        int total_dich ;
        list< TextureInfo > list_dan ;
        TextureInfo vien_dan;
        while(/*player.defense >0 && total_dich >0*/ true){
            SDL_RenderClear(renderer);
            drawHUD(renderer, font, level, point, highestpoint, player.defense);


           /* mouse =make_pair(-1,-1);
            SDL_RenderClear(renderer);
            handleEvent(upx,upy,mouse , ban_dan);

            //thực hiện vẽ xe tăng sau điều khiển
            lua_chon(renderer, xe , player , upx ,upy,mp);

            // nếu lựa chọn bắn đạn

            if(ban_dan && fps_ban_dan >=20){
                vien_dan.khoi_tao_dan(player,player.angle);
                list_dan.push_front(vien_dan);
                fps_ban_dan =0 ;
                ban_dan =false ;// bắn xong
            }

            else fps_ban_dan ++ ;
            */
            total_dich =0;
            for(int i= 10 ;i< so_luong_dich ; i++){
                if(enemy_list[i].id==0)continue ; // nếu xe tăng này đã bị hạ kiểm tra xe tiếp theo

                total_dich++; // biến đếm số lượng xe tăng địch còn lại

                SDL_Rect pre_rect = enemy_list[i].rect;// lưu vị trí hiện tại của xe tăng địch

                if(auto_act(than_dich,player , enemy_list[i],mp)){
                    vien_dan.khoi_tao_dan(enemy_list[i],enemy_list[i].angle_two);
                    //list_dan.push_front(vien_dan);
                }
                //Vẽ xe tăng địch
                print_enemy(renderer,enemy_list[i],pre_rect,mp);
            }

            // cộng điểm cho người chơi
            point += (level/5 +1)*(pre_dich-total_dich);
            pre_dich = total_dich ; // cập nhật số lượng địch thực tế hiện tại


            for (int i= 20 ;i< so_vat_can ;i++){
                if(obj_list[i].id==0)continue ;
                //vẽ vật cản
                render(obj_list[i].rect,0,renderer,tuong);
            }
            /*for(auto it = list_dan.begin(); it != list_dan.end(); ) {
                if(tuong_tac_dan(dan_surface, *it, obj_list, enemy_list, player, mp)) {
                    it = list_dan.erase(it); // Gán iterator mới từ erase
                }
                else {
                    render(it->rect, it->angle, renderer, dan_surface);
                    ++it; // Chỉ tăng iterator khi không xóa
                }


            } */
             SDL_RenderPresent(renderer);
             SDL_Delay(5);

        }

        // kết thúc màn hiện tại , lên level
        level++;

    }
}
