#ifndef ACT_H
#define ACT_H

#include<SDL2/SDL_ttf.h>


void new_obj_location(SDL_Rect &rect , int angle ,int step);


void handleEvent(int &upx , int &upy, std::pair<int,int> &mouse , bool & ban_dan);


 //SDL_Rect autoact(SDL_Renderer *renderer ,SDL_Rect &rest,SDL_Rect &mainrect,int upx,int upy , std::pair<SDL_Texture*, TextureInfo> *obj, pixelmp **mp);

void drawHUD(SDL_Renderer* renderer, TTF_Font* font, int level, int points, int highestPoint, int hearts);

void bfs_area(int x_max,int y_max , SDL_Rect &rect , int bfs_map[45][25]);

void handleEvent(int &upx , int &upy, std::pair<int,int> &mouse_left , bool & ban_dan);

bool kiem_tra_va_cham ( OBJ &obj , OBJ &player, OBJ *enemy_list , int &total_enemy , int bfs_map[45][25]);

bool Auto_ACT( OBJ &enemy , OBJ &player , OBJ *enemy_list,int bfs_map[45][25],int &total_enemy ,int & fps ,int &level);

bool kiem_tra_duong_dan (OBJ &obj ,OBJ &player, OBJ *enemy_list , int &total_enemy ,OBJ *wall_list , int wall_map[45][25]);

void lua_chon( OBJ &player , OBJ *enemy_list , int bfs_map[45][25],int &total_enemy, int fps , int &upx , int &upy);

#endif // ACT_H
