#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <utility>
#include <queue>
#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <SDL2/SDL_ttf.h>
#include "co_che.h"
#include "act.h"

using namespace std;

bool quit = true;//khi người dùng chưa ấn thoát

int find_path( SDL_Rect &rect , int bfs_map[45][25]){
    int ans =0;
    if(rect.x <0 || rect.x +rect.w > 1320 || rect.y <40 ||rect.y+rect.h >760){
        // trường hợp ngoài viền , loại bỏ luôn
        return 100000000;
    }
    vector<int> rect_Rows ={rect.x/40 , (rect.x+39)/40};
    vector<int> rect_Cols ={rect.y/40 , (rect.y+39)/40};
    for(int X :rect_Rows){
        for(int Y :rect_Cols){
            ans += abs(bfs_map[X][Y]);
        }
    }
    return ans ;
}

void bfs_area(int x_max, int y_max , SDL_Rect &rect , int bfs_map[45][25]){
    queue<pair<int,int>> Q; // tìm đường đi giữa địch và nhân vật
    queue<pair<int,int>> QQ ; // tìm khoảng cách giữa địch và nhân vật trường hợp sẽ phá tường
    vector<int> rect_Rows ={rect.x/40 , (rect.x+39)/40};
    vector<int> rect_Cols ={rect.y/40 , (rect.y+39)/40};
    for(int X :rect_Rows){
        for(int Y :rect_Cols){
            bfs_map[X][Y]=1;
            Q.push({X,Y});
        }
    }
    int a[]={1,-1,0,0};
    int b[]={0,0,1,-1};
    while(Q.size()||QQ.size()){
        if(Q.size()>0){
            int x=Q.front().first;
            int y=Q.front().second;
            Q.pop();
            for(int i=0;i<4;i++){

                int xx = x+a[i];
                int yy = y+b[i];
                if(0<=xx&&xx<33&&1<=yy&&yy<19){
                    if(bfs_map[xx][yy]==0){
                        bfs_map[xx][yy]=bfs_map[x][y]+1;
                        Q.push({xx,yy});
                    }
                    else if(bfs_map[xx][yy]==-1){
                        bfs_map[xx][yy]=-bfs_map[x][y]-3;
                        QQ.push({xx,yy}); // với trường hợp dích tường cập nhật vào QQ ;
                     }
                 }
            }

        }
        else{
            int x=QQ.front().first;
            int y=QQ.front().second;
            QQ.pop();
            for(int i=0;i<4;i++){
                int xx = x+a[i];
                int yy = y+b[i];
                if(0<=xx&&xx<33&&1<=yy&&yy<19){
                    if(bfs_map[xx][yy]==0){
                        bfs_map[xx][yy]=-bfs_map[x][y]+1;
                        Q.push({xx,yy});
                    }
                    else if(bfs_map[xx][yy]==-1){
                        bfs_map[xx][yy]= bfs_map[x][y]-1;
                        QQ.push({xx,yy}); // với trường hợp dích tường cập nhật vào QQ ;
                    }
                }
            }

        }
    }
}




void new_obj_location(SDL_Rect &rect , int angle ,int step){
    // vị trí mới sau di chuyển
    if( angle==0)rect.x+=step;
    else if(angle==90)rect.y+=step;
    else if(angle==180)rect.x-=step;
    else rect.y-=step;
}
bool so_sanh( SDL_Rect a , SDL_Rect b){
    if(a.x!=b.x)return false ;
    if(a.y!=b.y)return false ;
    if(a.w!=b.w)return false ;
    if(a.h!=b.h)return false ;
    return true;
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



bool kiem_tra_va_cham (OBJ &obj ,OBJ &player, OBJ *enemy_list , int &total_enemy , int bfs_map[45][25]){

    int x = obj.rect.x;
    int y = obj.rect.y;
    int w = obj.rect.w;
    int h = obj.rect.h;

    if(x<0||x+w>1320||y<40||y+h >760){
        return true;
    }

     w = (x+ w-1)/40 ; h = ( y+ h-1)/40 ;
     x/=40; y/=40 ;

    if(bfs_map[x][y]<0||bfs_map[x][h]<0||bfs_map[w][y]<0||bfs_map[w][h]<0){

        return true ;
    }

    if(obj.id!=player.id){
        if(obj.tiep_xuc(player))return true;
    }
    for(int i= 1 ; i< total_enemy ;i++){
       if(enemy_list[i].id == -1)continue ;
       if(obj.id!=enemy_list[i].id && obj.tiep_xuc(enemy_list[i]))return true;
    }
    return false ;
}


bool Auto_ACT( OBJ &enemy , OBJ &player , OBJ *enemy_list,int bfs_map[45][25],int &total_enemy ,int & fps ,int &level){
    if(enemy.clock == 90){
        srand( enemy.rect.x*enemy.rect.y + enemy.angle);
        enemy.angle = 90*(rand()%4);
        enemy.clock = rand()%20 ;
    }
    enemy.clock++;
    bool shot = false ; // có nên bắn đạn không
    //Tạo bản sao của địch
    OBJ enemy_test = enemy ;

    // Lấy vecto hướng
    int x = player.rect.x - enemy.rect.x;
    int y = player.rect.y - enemy.rect.y;


    // Tính khoảng cách từ địch đến nhân vật
    int dist = distance(player.rect.x,player.rect.y,enemy.rect.x,enemy.rect.y);


    if((enemy.rand_shot == fps)){
        if(level <3){
            // Ở level thấp địch bắn ít đạn
            srand(enemy.rect.x);
            shot = static_cast<bool>(rand()%2);
        }
        else shot =true ;
    }

    // tạo hạt nhân cho random
    srand( enemy.rect.x-enemy.rect.y + enemy.angle);

    int choose = rand()%9 + min(2, level/3); // random lấy số để xác định có đuổi không (>7)

    if(dist <= 150 + min(level*20,200)||choose >7){
         if((enemy.rand_shot == fps)||((enemy.rand_shot == (fps + 15)% 30 && level > 4))){
            //bắn khoảng 2 lần /s
            if(dist <= 150 +min(level*20,200) ){
                shot = true ;
                // kiểm tra lại dist để đảm bảo nó nằm trong bán kính cho phép
            }
         }
         // tạo hạt nhân cho random
         srand(enemy.rect.y);
         //tiếp tục lấy số lần nữa
         choose = choose/7*7 + rand()%10 + min(4,level/2) + max(0 ,3-dist/50) ; // random lấy số để xác định có đuổi không (>7)

         if(choose > 7){
            int score_area = find_path(enemy.rect, bfs_map);
            //cout<< player.rect.x<<' '<<player.rect.y<<' '<<enemy.rect.x<<' '<<enemy.rect.y<<' '<<enemy.angle<<'\n';
            int angle = enemy.angle ;
            SDL_Rect pick_rect = enemy.rect;
            for(int i=0;i<4;i++){

                if(max(abs(x),abs(y))<50 + min(20,level*3)){
                    // khi địch ở gần
                    //cout<<x<<' '<<y;
                    if(x==0){
                        //cout<<"x"<<'\n';
                        if(y>0)angle = 90;
                        else angle = 270 ;
                        break;
                    }
                    else if(y==0){
                        //cout<<"y"<<'\n';
                        if(x>0)angle = 0;
                        else angle =  180;
                        break;
                    }
                    else{

                        if(abs(x)<=abs(y)){

                            enemy_test = enemy;
                            enemy_test.rect.x += 2*x/abs(x);
                            if(kiem_tra_va_cham(enemy_test, player , enemy_list , total_enemy ,bfs_map)==false){
                                pick_rect = enemy_test.rect;
                                if(x>0)angle =0;
                                else angle = 180;
                                break;
                            }
                        }
                        else {
                            enemy_test = enemy ;
                            enemy_test.rect.y += 2*y/abs(y);
                            if(kiem_tra_va_cham(enemy_test, player , enemy_list , total_enemy ,bfs_map)==false){
                                pick_rect = enemy_test.rect;
                                if(y>0)angle =90;
                                else angle = 270;
                                break;
                            }
                        }
                    }

                }
                enemy_test = enemy;
                new_obj_location(enemy_test.rect,(enemy.angle +i*90)%360,2);
                int new_score_area = find_path(enemy_test.rect, bfs_map);
                //cout<< enemy_test.rect.x <<' '<<enemy_test.rect.y<<' '<<find_path(enemy_test.rect, bfs_map);
                if(new_score_area < score_area ||(new_score_area == score_area && so_sanh(pick_rect,enemy.rect))){

                    angle = (enemy.angle + i*90)%360;
                    score_area = new_score_area ;
                    if(kiem_tra_va_cham(enemy_test, player , enemy_list , total_enemy ,bfs_map)==false){
                        pick_rect = enemy_test.rect;
                        //cout<<" TRUE ";

                    }

                }

            }
            //cout<<'\n';
            enemy.rect = pick_rect;
            enemy.angle = angle ;
            return shot ;
         }


    }
    for(int i=0;i<4;i++){
        enemy_test = enemy ;
        enemy_test.angle = (enemy.angle + 90*i)%360 ;
        new_obj_location(enemy_test.rect,enemy_test.angle,2);
        if(kiem_tra_va_cham(enemy_test,player,enemy_list,total_enemy,bfs_map)==false){
            enemy = enemy_test ;

            return shot;
        }
    }

    return shot ;
}

bool kiem_tra_duong_dan (OBJ &obj ,OBJ &player, OBJ *enemy_list , int &total_enemy ,OBJ *wall_list , int wall_map[45][25]){
    bool ans = false;
    int x = obj.rect.x;
    int y = obj.rect.y;
    int w = obj.rect.w;
    int h = obj.rect.h;

    if(x<0||x+w>1320||y<40||y+h >760){
        return true;
    }

     w = (x+ w-1)/40 ; h = ( y+ h-1)/40 ;
     x/=40; y/=40 ;

    if(wall_map[x][y]>0){
        ans = true;
        wall_list[wall_map[x][y]].defense-=1;
    }
    if(wall_map[x][h]>0&&(h!=y)){
        ans = true;
        wall_list[wall_map[x][h]].defense-=1;
    }
    if(wall_map[w][y]>0 && (x!=w)){
        ans = true;
        wall_list[wall_map[w][y]].defense-=1;
    }
    if(wall_map[w][h]>0 && (x!=w)&&(h!=y)){
        ans = true;
        wall_list[wall_map[w][h]].defense-=1;
    }

    if(obj.attribute!=player.attribute){
        if(obj.tiep_xuc(player)){
            ans =true;
            player.defense-=1;
        }
    }
    for(int i= 1 ; i< total_enemy ;i++){
       if(enemy_list[i].id == -1)continue;
       if((enemy_list[i].attribute != obj.attribute ) && obj.tiep_xuc(enemy_list[i])){
            enemy_list[i].id = -1; // bị trúng đạn xóa xe tăng địch
            ans =true;
       }
    }
    return ans;
}


void lua_chon( OBJ &player , OBJ *enemy_list , int bfs_map[45][25],int &total_enemy, int fps , int &upx , int &upy){
    if(fps%5 >0){
        // sau 5 fps thì chạy một lần ;
        return ;
    }
    if(abs(upx)>= abs(upy)){
        if(upx >0)player.angle = 0;
        else if ( upx <0)player.angle =180;
    }
    else{
        if(upy >0)player.angle = 270 ;
        else if( upy <0)player.angle =  90 ;
    }
    if(abs(upx)+abs(upy)>0){
        OBJ player_test = player ;
        new_obj_location(player_test.rect,player_test.angle,10);
        if(kiem_tra_va_cham(player_test ,player_test, enemy_list , total_enemy , bfs_map)==false){
            player = player_test;
        }
    }
    upx = upy =0;
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
