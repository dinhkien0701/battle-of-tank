
#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <utility>
#include <algorithm>
#include "loading.h"
#include "menu.h"
#include "co_che.h"
#include "act.h"


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

    /*loadingstart(window,renderer);
    menustart(window,renderer); */
    run_game(window,renderer);
    SDL_DestroyWindow(window);



    return 0;
}
