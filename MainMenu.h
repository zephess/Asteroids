#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "Button.h"
#include "SDL.h"
#include "SDL_ttf.h"

class MainMenu {
public:
    MainMenu(SDL_Renderer* renderer);
    ~MainMenu();
    void handleEvent(SDL_Event& e);
    void render();
    bool isGameStarted() const;
    bool shouldQuit() const;

private:
    SDL_Renderer* renderer;
    TTF_Font* ghastly_font;
    SDL_Texture* backgroundTexture;
    Button playButton;
    Button quitButton;
    bool gameStarted;
};

#endif // MAIN_MENU_H
