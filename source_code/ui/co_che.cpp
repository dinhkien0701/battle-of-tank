#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <random>
#include <utility>  // Thêm thư viện này để sử dụng std::pair
#include <algorithm>
#include <thread>
#include <fstream>
#include <atomic> //giúp đọc/ghi biến một cách nguyên tử (atomic operation), đảm bảo không bị gián đoạn khi có luồng khác truy cập.
#include "co_che.h"
#include "act.h"
using namespace std;

std::atomic<bool> running(true);  // Kiểm soát vòng lặp , đảm bao gpu hoạt động trơn chu

int read_high_point(){
    ifstream file("C:/SDL2_Game/Point/high_point.txt");
    int number ;
    file>>number;
    file.close();
    return number ;
}

void write_point(int point){
    ofstream file("C:/SDL2_Game/Point/high_point.txt", std::ios::trunc); // mở file ở chế độ ghi , xóa hết nội dung trong file
    file<<point;
    file.close();
}

int distance(int &x1,int &y1,int &x2, int &y2){
    return static_cast<int>(sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)));
}

void dfs_map(int i, int j , int map_of_level[45][25],int &total , int max_total){
    map_of_level[i][j]=1;
    int res = 0;
    srand(i*j + (i-j)*(j-i));
    for(int num = rand()%3 +1; num >0; num--){
        if(res == 2||total>=max_total)continue;
        int x = rand()%2 - rand()%2 ;
        int y = rand()%2 - rand()%2 ;
        if(x<0||x>32||y<1||y>18||map_of_level[i+x][j+y])continue;
        dfs_map(i+x,j+y,map_of_level,++total,max_total); // ++total : thêm giá trị mới truyền , total++ có thể lỗi

    }
}
void map_khoi_dong( int level , OBJ *make_enemy , OBJ *make_obj , OBJ &player , int &sum_enemy , int &sum_wall,int wall_map[45][25]){
    // khoiwt tạo mảng ban đầu để random map
    int mapp[45][25];
    for(int i=0;i<45;i++){
        for(int j=0;j<25;j++){
            mapp[i][j]=0;
        }
    }

    /*
      1: tường
      2: địch
      3: main
      */
    if(level <3){
        mapp[20][10]=3; // vị trí ban đầu của người chơi
        mapp[2][2]=2;   // vị trí ban đầu của địch
        for(int i=0;i<= level;i++){
            for(int j=0;j<=level;j++){
                mapp[5*(i+1)][5*(j+1)]=1;
            }
        }
    }

    else {
         if(level ==3){
            mapp[30][15]=3; // vị trí ban đầu của người chơi
            mapp[2][2]=mapp[17][2]= mapp[25][4]=2;
         }
         else{
            // level >= 4 ;

            // cx, cy vị trí ô nhân vật chính
            int cx= 2+ rand()%30; // random từ 2 ->31
            int cy= 2+ rand()%14; // random từ 2 ->15;
            mapp[cx][cy]=3;

            for(int i= min(30,level*2 + level/5 + level/6 + level/7 +level/9);i>0;i-- ){
                int x,y;

                do{
                    x =2+ rand()%30; // rand từ 2->31
                    y =2+ rand()%15; // rand từ 2->16
                } while(mapp[x][y]>0 || sqrt((cx-x)*(cx-x)+(cy-y)*(cy-y))<9);// rand đến khi tìm đucợ vị trí trống và cách đủ xa
                mapp[x][y] = 2; // vị trí kẻ địch khi random
            }
         }
         int max_total =level*15 + (level - 5)*10; // số bức tường tương ứng level
         max_total = min(260,max_total); // tối đa 260 bức tường
         int total =0;
         while(total<max_total){
            int i =2+ rand()%30;  // rand từ 2->31
            int j =2+ rand()%17;  // rand từ 2->18
            if(mapp[i][j]==0){
                dfs_map(i,j,mapp,++total, max_total);
            }
         }
    }
    //lấy các đối tượng đã được tạo
    sum_enemy =1;
    sum_wall  =1;
    for(int i=0;i<33;i++){
        for(int j=1;j<19;j++){
            if(mapp[i][j]>0){
                SDL_Rect rect = {40*i,40*j,40,40};
                if(mapp[i][j]==1){
                    make_obj[sum_wall].khoi_tao_tuong(sum_wall,rect); // tạo chướng ngại vật
                    wall_map[i][j] = sum_wall ;
                    sum_wall++;
                }
                else if(mapp[i][j]==2){

                    make_enemy[sum_enemy].khoi_tao_dich(sum_enemy,rect);
                    make_enemy[sum_enemy].angle = 270;
                    sum_enemy++;

                }
                else if(mapp[i][j]==3){
                    // mã nhân dạng ID nhân vật là 0;

                    player.khoi_tao_nhan_vat(0,player.defense+1,rect);

                    player.angle = 270;
                    //nhân vật thêm một mạng mỗi lượt chơi , tối đa tích trữ 3 mạng
                }

            }
        }
    }

}
