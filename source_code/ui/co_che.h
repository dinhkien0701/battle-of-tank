#ifndef CO_CHE_H
#define CO_CHE_H
#include<cmath>
//tiền khai báo
class pixelmp{
    public:
        int alpha ;// Do trong suot
        double speed ;// Hệ số làm chậm của môi trường
        int damage ;// Sat thuong tai pixel
        int attribute;// thuộc tính của đối tượng đang nắm giữ pixel
        void initialize(int al ,int sp, int dam,int att){
            alpha=al;
            speed=sp;
            damage =dam;
            attribute=att;
        }
};

struct TextureInfo{
    int root_id =1;// id gốc của đối tượng
    int id =1;//id của png được dùng hiện tại
    int scope =1;// phạm vi các khung ảnh của đối tượng
    int angle =-90; // góc của hướng hiện tại đơn vị (độ);
    int speed = 3 ;// Tóc độ cơ bản của đối tượng
    double scale =1.0 ; // Tỉ lệ tốc độ hiện tại
    int attribute =1; // thuộc tính của đối tượng
    int damage =0; // sát thương đối tượng
    int defense =0;// giáp của đối tượng
    SDL_Rect rect;//

};
void heart(int &cx, int &cy ,int x, int y, int w, int h);
double to_radian(int angle);
int to_degree(double angle);
void xoay_vector(int &ansX, int &ansY ,int x, int y, int cx, int cy , int angle );
void mapping(int &ansX, int &ansY , int cx, int cy , int px , int py , double scaleX, double scaleY);
void layer(SDL_Renderer *renderer,SDL_Surface *surface, TextureInfo &info, pixelmp **mp);

inline int distance(std::pair<int,int> v1, std::pair<int,int> v2){
    return static_cast<int>(sqrt((v1.first-v2.first)*(v1.first-v2.first)+(v1.second-v2.second)*(v1.second-v2.second)));
}

bool sol(SDL_Surface *surface, TextureInfo &info, pixelmp **mp);

#endif // CO_CHE_H
