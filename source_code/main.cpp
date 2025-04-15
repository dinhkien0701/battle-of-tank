
#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <utility>
#include <algorithm>
#include "loading.h"
#include "menu.h"
#include "co_che.h"
#include "act.h"
#include "music.h"

int main(int argc, char* argv[]) {
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");// su dung noi suy de anh dep

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (TTF_Init() == -1) {
    std::cout << "TTF_Init Error: " << TTF_GetError() << std::endl;
    SDL_Quit();
    return -1;
    }
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return -1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Failed to initialize SDL Mixer: %s\n", Mix_GetError());
        return -1;
    }
    SDL_Window* window = SDL_CreateWindow("Tank Gun Battle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,800, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // tao renderer
    if (renderer == nullptr) {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Sound background_music_menu = Sound("music/background_music_menu.mp3",2);
    Sound background_music_game = Sound("music/background_music_game.mp3",2);

    Sound wall_destroy = Sound("music/wall_destroy.mp3",1);
    Sound shot_music = Sound("music/shot.mp3",1);
    Sound tank_destroy = Sound("music/tank_destroy.mp3",1);
    Sound pick_mouse  = Sound("music/pick_banner.mp3",1);
    Sound crash   = Sound("music/crash.mp3",1);

    background_music_menu.play(); // khởi động nhạc nền
    Mix_VolumeMusic(32); // đặt âm lượng nhạc nền phù hợp
    loadingstart(window,renderer);
    while(true){
        background_music_menu.play(); // tiếp tục bật nhạc nền nếu ta quay lại cửa sổ này
        SDL_SetWindowSize(window,800,800); // thay đổi kích thước của sổ
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED); // căn giữa cửa sổ
        menustart(window,renderer,pick_mouse);

        background_music_menu.stop();
        SDL_Delay(500) ; // tạm dừng 0.5s
        background_music_game.play();
        run_game(window,renderer,wall_destroy,tank_destroy,shot_music,crash);
        background_music_game.stop();
        SDL_Delay(1000); // tạm dừng 1s
    }

    SDL_DestroyWindow(window);



    return 0;
}
