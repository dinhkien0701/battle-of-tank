#include <iostream>
#include <queue>
#include <iomanip>
#include <cstring>
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
SDL_Surface *nen_surface = IMG_Load("image/background.jpg");
    // Main
SDL_Surface *nhan_vat = IMG_Load("image/xe.png");

    //Đich
SDL_Surface *enemy = IMG_Load("image/xe_dich.png");


    //Tường thành

SDL_Surface *wall = IMG_Load("image/tuong.png");

   //Đạn
SDL_Surface *bullet_one = IMG_Load("image/dan_do.png");
SDL_Surface *bullet_two = IMG_Load("image/dan_den.png");


void wall_to_bfs ( OBJ &player , int wall_map[45][25], int bfs_map[45][25]){
    for(int i=0;i<45;i++){
        for(int j=0 ;j < 25 ;j++){
            if(wall_map[i][j]>0)bfs_map[i][j]=-1;
            else bfs_map[i][j]=0;
        }
    }
    bfs_area( 32, 18, player.rect , bfs_map);
}


void run_game(SDL_Window *window , SDL_Renderer *renderer , Sound &wall_destroy , Sound &tank_destroy , Sound &shot_music , Sound &crash){

    bool back_to_menu = false ; // khởi tạo nút thoát khỏi màn chơi

    //làm sạch thao tác
    SDL_Event event;
    while(SDL_PollEvent(&event));// xoa het cac thao tac phim thua

    SDL_Texture* Background = SDL_CreateTextureFromSurface(renderer,nen_surface);
    SDL_Texture* Nhan_vat = SDL_CreateTextureFromSurface(renderer,nhan_vat);
    SDL_Texture* Enemy    = SDL_CreateTextureFromSurface(renderer,enemy);
    SDL_Texture* Wall     = SDL_CreateTextureFromSurface(renderer,wall);
    SDL_Texture* Bullet_one   = SDL_CreateTextureFromSurface(renderer,bullet_one);
    SDL_Texture* Bullet_two   = SDL_CreateTextureFromSurface(renderer,bullet_two);

    TTF_Font* font = TTF_OpenFont("font/OpenSans.ttf", 22);
    TTF_Font* font_end = TTF_OpenFont("font/OpenSans.ttf", 40);

    int window_w , window_h;
    SDL_SetWindowSize(window,1320,760); // thay đổi kích thước của sổ
    SDL_Rect background_rect = {0,0,1320,760};
    // Căn giữa cửa sổ cho phù hợp
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    SDL_GetWindowSize(window , &window_w, &window_h);
    // Khởi tạo map của bức tường
    int wall_map[45][25];
    memset(wall_map, 0, sizeof(wall_map));
    int bfs_map[45][25];
    memset(bfs_map, 0, sizeof(bfs_map));

    int level = 1 ;
    int point = 0;
    int highestpoint=  read_high_point(); // đọc điểm cao nhất đã từng đạt được
    OBJ *enemy_list = new OBJ[50];
    OBJ *wall_list  = new OBJ[400];
    OBJ player;
    int so_luong_dich, so_vat_can ;
    bool space =false ;
    int fps = 0 ;
    player.defense = 2; // người chơi ban đầu có 3 ( 2+ 1 mạng lúc tạo map )
    bool start = true ; // có phải người chơi bắt đầu chơi không
    while(player.defense>0 && back_to_menu ==false){

        SDL_RenderClear(renderer); // làm sạch bút vẽ

        // làm sạch bản đồ
        memset(wall_map, 0, sizeof(wall_map));
        memset(bfs_map, 0, sizeof(bfs_map));

        SDL_RenderCopy(renderer,Background,NULL,&background_rect);

        // Khởi tạo hạt giống cho hàm rand để sinh ngẫu nhiên
        srand(std::time(0));

       // khởi tạo map
        map_khoi_dong( level , enemy_list , wall_list , player ,so_luong_dich , so_vat_can ,wall_map);

        //player.rect.x = 510; player.rect.y =290;
        //enemy_list[2].rect.x = 520 ; enemy_list[2].rect.y =246;

       // in hình nhân vật
        player.print_obj(Nhan_vat,renderer);

        // in hình địch
        for(int i=1;i<so_luong_dich;i++){
            enemy_list[i].print_obj(Enemy,renderer);

        }

        // in hình vật cản ( tường )
        for (int i=1;i<so_vat_can;i++){

            //vẽ tường
            wall_list[i].print_obj(Wall,renderer);

        }

         wall_to_bfs(player,wall_map,bfs_map);
         /*for(int i=1;i<=18;i++){
            for(int j=0;j<=32;j++)cout<<setw(2)<<wall_map[j][i]<<' ';
            cout<<'\n';
         }*/

        //cout<<player.rect.x<<" "<<player.rect.y<<" "<<player.rect.w<<" "<<player.rect.h;

        draw_banner(renderer, font, start);
        SDL_RenderPresent(renderer);

        /*if(enemy_list[2].tiep_xuc(player)==false){
            cout<<" aaaaaaaaaaaaaaaaa";
        }*/

        //Khởi tạo các thành phần cho hàm HandleEvent()
        pair<int,int> mouse;
        int upx=0,upy=0;
        bool pause = false ; // khởi tạo nút tạm dừng


        int total_dich = so_luong_dich - 1;
        int pre_dich = total_dich ;
        queue<OBJ> list_bullet;
        OBJ vien_dan ;

        // banner giới thiệu
        // tận dụng luôn biến space
        do{
            handleEvent(upx,upy,mouse,space , pause , back_to_menu);

        }while( space == false);

        space = false ; // đặt lại giá trị space
        back_to_menu = false ;// đặt lại giá trị nút M
        //làm sạch thao tác
        SDL_Event event;
        while(SDL_PollEvent(&event));// xoa het cac thao tac phim thua
        while(player.defense >0 && total_dich >0 && back_to_menu ==false){

            int present_defense = player.defense ; //lưu số mạng hiện tại của người chơi

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer,Background,NULL,&background_rect);

            drawHUD(renderer, font, level, point, highestpoint, player.defense);


            mouse =make_pair(-1,-1);

            handleEvent(upx,upy,mouse , space , pause , back_to_menu );

            //thực hiện vẽ xe tăng sau điều khiển
            lua_chon(player ,enemy_list , bfs_map, so_luong_dich, fps , upx ,upy);
            player.print_obj(Nhan_vat,renderer);
            // nếu lựa chọn bắn đạn

            if(space && player.clock >= 15){
                vien_dan.khoi_tao_dan(player);
                list_bullet.push(vien_dan);
                // bắn xong reset ;
                player.clock =1;
                space =false ;// bắn xong
                shot_music.play();
            }

            else player.clock ++ ;

            total_dich =0;
            for(int i= 1 ;i< so_luong_dich ; i++){
                if(enemy_list[i].id==-1)continue ; // nếu xe tăng này đã bị hạ kiểm tra xe tiếp theo

                else if(enemy_list[i].id==-2){
                    // nếu xe tăng vừa bị hạ;
                    tank_destroy.play(); // phát âm thanh
                    enemy_list[i].id =-1 ;// đặt id =-1 để không phát âm thanh lần nữa
                    continue;
                }

                total_dich++; // biến đếm số lượng xe tăng địch còn lại



                if(Auto_ACT(enemy_list[i],player ,enemy_list,bfs_map,so_luong_dich ,fps,level)){
                    vien_dan.khoi_tao_dan(enemy_list[i]);
                    list_bullet.push(vien_dan);
                }
                //Vẽ xe tăng địch
                enemy_list[i].print_obj(Enemy,renderer);
            }

            // cộng điểm cho người chơi
            point += (level/5 +1)*(pre_dich-total_dich);
            pre_dich = total_dich ; // cập nhật số lượng địch thực tế hiện tại


            for (int i= 1 ;i< so_vat_can ;i++){
                if(wall_list[i].id ==-1)continue ;
                if(wall_list[i].defense <=0){
                    wall_destroy.play();
                    wall_list[i].id = -1; // xóa bằng cách đưa id về -1
                    wall_map[wall_list[i].rect.x/40][wall_list[i].rect.y/40]=0;
                    continue;
                }
                wall_list[i].print_obj(Wall,renderer);
            }
            // cập nhật lại bản đồ
            wall_to_bfs(player,wall_map,bfs_map);

            // Đối với đạn

            for(int i =(int)list_bullet.size();i>0;i--){
                vien_dan = list_bullet.front();
                new_obj_location(vien_dan.rect,vien_dan.angle,4);
                list_bullet.pop();
                if(kiem_tra_duong_dan(vien_dan, player, enemy_list , so_luong_dich ,wall_list , wall_map)==false){
                    list_bullet.push(vien_dan);
                }
                // vẽ hình đạn
                if(vien_dan.attribute == 1){
                    // nếu là địch
                    vien_dan.print_obj(Bullet_two,renderer);
                }
                else{
                    // nếu là nhân vật
                    vien_dan.print_obj(Bullet_one,renderer);
                }
            }
            if(present_defense>player.defense){
                //nếu người chơi bị giảm mạng -> bị bắn trúng , phát tiếng
                crash.play();
            }
             SDL_RenderPresent(renderer);
             if(fps == 60)fps =0; // hoàn thành một vòng chu trình
             else fps ++;
             SDL_Delay(30);

             // nếu người chơi muốn tạm dừng
             if(pause ==true){
                SDL_Rect rect_pause = {0,0,1320,40};
                menu_HUD(renderer, font, rect_pause, "Đang tạm dừng nhấn phím P để tiếp tục chơi !");
                SDL_RenderPresent(renderer);
                do{
                    SDL_Delay(30);
                    handleEvent(upx,upy,mouse , space , pause ,back_to_menu);
                    upx=upy =0;
                }while((pause==true)&&(back_to_menu ==false));
             }

             if(point > highestpoint){
                //cập nhật điểm cao theo thời gian thực
                write_point(point);
                highestpoint = point ;
             }
        }




        if(player.defense ==0){
            // khi người chơi kết thúc ván đấu
            drawHUD(renderer, font, level, point, highestpoint, player.defense); // render lại kết quả cuối cùng

            SDL_Rect rect_over = {500,350,350,80};
            menu_HUD(renderer, font_end, rect_over, "GAME OVER !");

            SDL_Rect rect_to_menu = {500,460,350,40};
            menu_HUD(renderer, font, rect_to_menu, "Nhấn phím M hoặc SPACE để trở về MENU");
            SDL_RenderPresent(renderer);
            SDL_Delay(1000);
            space = false ;
            do{
                SDL_Delay(30);
                handleEvent(upx,upy,mouse , space , pause ,back_to_menu);

            }while(back_to_menu == false && space == false);

        }

        // kết thúc màn hiện tại , lên level , tặng  số điểm   = level;

        point += level;
        level++;
        start = false ; // bây giừ là tiếp tục chơi rồi , nên đổi lại giá trị start
    }


}
