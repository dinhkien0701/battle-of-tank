#ifndef CO_CHE_H
#define CO_CHE_H
#include<cmath>
#include<iostream>
#include <ctime>
#include "music.h"
//tiền khai báo

struct OBJ{
    int id ;// mã nhận dạng đối tượng
    int angle ; // góc của hướng hiện tại của đối tượng ( độ );
    int attribute; // thuộc tính của đối tượng

    int defense ;// giáp của đối tượng

    int rand_shot; // tại khung hình thứ bao nhiêu trên 60 sẽ bắn đạn ;

    int clock ;

    SDL_Rect rect;// vị trí của đối tượng


    void khoi_tao_nhan_vat (int ID ,int Defense , SDL_Rect Rect){
        id = ID; // mã nhận dạng của nhân vật ;
        attribute = 2; // 2 là nhân vật
        rect = Rect;
        defense =Defense;
        angle = 270 ;
        clock = 15 ; // cơ bắn của nhân vật là tích đủ 15 fps / lần bắn
    }
    void khoi_tao_dich(int ID, SDL_Rect Rect){
        id = ID;
        attribute =1; // 1 là địch
        rect = Rect ;
        defense = 1;
        angle =270;

        // Khởi tạo hạt giống cho hàm rand để sinh ngẫu nhiên
        srand(ID*rect.x + rect.y);
        rand_shot = rand()%60 ; // cơ chế bắn của địch là khoảng  2s/ lượt bắn , khi ở lượt rand_shot / 60;
        srand(std::time(0));
        clock = rand()%20; // cơ chế luôn hướng
    }

    void khoi_tao_tuong (int ID , SDL_Rect Rect){
        // khơi tạo chướng ngại vật ( tường )
        id = ID; // mã nhận dạng đối tượng
        attribute = 3; // 0 là trướng ngại vật
        rect = Rect ; // vị trí bức tường
        defense = 3 ; // ban đâu bức tường có 3 giáp
        angle = 0 ;

    }
    void khoi_tao_dan ( OBJ &obj ){
        // khởi tạo đạn
        rect = {obj.rect.x,obj.rect.y,10,10};
        angle = obj.angle;
        attribute = obj.attribute ; // loại đạn tương đương loại đối tượng tạo ra viên đạn đó
        if(angle == 0){
            rect.x+=obj.rect.w;
            rect.y+=15;
        }
        else if(angle==90){
            rect.x+=15;
            rect.y+=obj.rect.h;
        }
        else if(angle==180){
            rect.x-=10;
            rect.y+=15;
        }
        else{
            rect.x+=15;
            rect.y-=10;
        }
    }
    void xoa_obj (){
        // loại bỏ một đối tượng , đưa id của đối tượng đó về -1
        id = -1;
    }

    bool tiep_xuc( OBJ &obj){
        // kiểm tra hai đối tượng có tiếp xúc không
        // true : có tiếp xúc
        // false : không tiếp xúc
        SDL_Rect b_rect = obj.rect;
        if(obj.id == -1){
            //nếu đối tượng đã bị xóa
            return false ;
        }
        if((rect.x<=b_rect.x&&rect.x+rect.w>b_rect.x)||(b_rect.x<=rect.x&&b_rect.x+b_rect.w>rect.x)){
            if((rect.y<=b_rect.y&&rect.y+rect.h>b_rect.y)||(b_rect.y<=rect.y&&b_rect.y+b_rect.h>rect.y)){
                return true ;
            }
        }
        return false ;
    }

    void print_obj(SDL_Texture *texture , SDL_Renderer *renderer){
        SDL_Point center = { rect.w / 2, rect.h / 2 };
        SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, &center, SDL_FLIP_NONE);
    }

};


int read_high_point();

void write_point(int point);


int distance(int &x1,int &y1,int &x2, int &y2);


void dfs_map(int i, int j , int map_of_level[45][25],int &total , int max_total);

void map_khoi_dong( int level , OBJ *make_enemy , OBJ *make_obj , OBJ &player , int &sum_enemy , int &sum_wall,int wall_map[45][25]);

void run_game(SDL_Window *window , SDL_Renderer *renderer , Sound &wall_destroy , Sound &tank_destroy , Sound &shot_music , Sound &crash);

#endif // CO_CHE_H
