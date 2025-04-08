#ifndef CO_CHE_H
#define CO_CHE_H
#include<cmath>
#include<iostream>
//tiền khai báo
class pixelmp{
    public:
        int alpha ;// Do trong suot
        int attribute;// thuộc tính của đối tượng đang nắm giữ pixel
        void initialize(int al ,int att){
            alpha=al;
            attribute=att;
        }
};

struct TextureInfo{
    int id ;//id của png được dùng hiện tại
    int angle ; // góc của hướng hiện tại đơn vị (độ);
    int attribute; // thuộc tính của đối tượng
    int defense ;// giáp của đối tượng
    int damage ;
    SDL_Rect rect;//
    int adjec[4]; // băng ghi hành động trong một giây
    int maxadjec[3];// số hành động mỗi loại tối đa trong một giây
    int angle_two ;
    void khoi_tao_dich(int ro_id , SDL_Rect rectt ){
        id =ro_id;
        attribute = 1; // 1 là địch
        rect =rectt;
        defense=1;
        //angle = 45; angle_two = 45 ; // góc ban đầu thân , súng

        for(int i=0;i<4;i++)adjec[i]=0;
        maxadjec[0]=40; maxadjec[1]=8 ; maxadjec[2]=12 ; // đứng im - xoay - tiến lùi

        maxadjec[3]= 0; // random để có bắn đạn hay không khi ở cự ly gần

    }
    void khoi_tao_nhan_vat (int ro_id ,int defens , SDL_Rect rectt){
        id =ro_id;
        attribute =2; // 2 là nhân vật
        rect =rectt;
        defense =defens;
        //angle = angle_two = 90 ; // góc ban đầu thân , súng
        adjec[0]=20 ; // tích đủ 20 fps ( 1/3 giây ) có thể bắn ( mỗi giây bắn tối đa 3 lần )
    }
    void khoi_tao_tuong (int ro_id , SDL_Rect rectt){
        // khơi tạo chướng ngại vật ( tường )
        id =ro_id; // mã vị trí trong mảng
        attribute = 3; // 0 là trướng ngại vật
        rect =rectt;
        defense=6;

    }
    void khoi_tao_dan ( TextureInfo &info , int Angle){
        // khởi tạo đạn

        rect = info.rect; // đạn được tạo bởi đối tượng nào thì có vị trí gần đối tượng đó
        rect.x += static_cast<int> (cos(3.0*Angle/180*3.1415));
        rect.y += static_cast<int> (sin(3.0*Angle/180*3.1415));
        angle  = Angle ;
        damage = info.attribute;
    }
    void xoa_obj (){
        id =0;
    }

};

void heart(int &cx, int &cy ,int x, int y, int w, int h);

double to_radian(int angle);

int to_degree(double angle);

void xoay_vector(int &ansX, int &ansY ,int x, int y, int cx, int cy , int angle );

void mapping(int &ansX, int &ansY , int cx, int cy , int px , int py , double scaleX, double scaleY);

void layer(SDL_Surface *surface, TextureInfo &info, pixelmp **mp);

void layer2(SDL_Renderer *renderer,SDL_Surface *surface, TextureInfo &info, pixelmp **mp);

int distance(int &x1,int &y1,int &x2, int &y2);

bool sol(SDL_Surface *surface, SDL_Rect &new_rect,int root_id,int angle , pixelmp **mp);

void map_khoi_dong( int level , TextureInfo *make_enemy , TextureInfo *make_obj , TextureInfo &mainn , int &sum_enemy , int &sum_obj);

void run_game(SDL_Window *window , SDL_Renderer *renderer);

void mapping_dich( SDL_Surface*surface, TextureInfo & enemy, pixelmp **mp);

bool tuong_tac_dan(SDL_Surface *surface, TextureInfo & info, TextureInfo *tuong ,TextureInfo *dich , TextureInfo &main , pixelmp **mp);

#endif // CO_CHE_H
