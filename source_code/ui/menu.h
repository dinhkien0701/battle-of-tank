#ifndef MENU_H
#define MENU_H
#include<SDL2/SDL_ttf.h>
#include<string>
void menustart(SDL_Window *window,SDL_Renderer *renderer);
void menu_HUD(SDL_Renderer* renderer, TTF_Font* font, SDL_Rect rect, std::string text);
#endif // MENU_H

