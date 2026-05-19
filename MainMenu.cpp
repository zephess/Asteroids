#include "MainMenu.h"
#include "Button.h"
#include "SDL_image.h"
#include <iostream>
#include "Main.h"

Button myButton;

MainMenu::MainMenu(SDL_Renderer* renderer)
    : renderer(renderer), ghastly_font(nullptr), backgroundTexture(nullptr), gameStarted(false) {

    // Load background image
    SDL_Surface* loadedSurface = IMG_Load("assets/images/bg_outside.png");
    if (!loadedSurface) {
        std::cout << "Failed to load background image! SDL_image Error: " << IMG_GetError() << std::endl;
    }
    else {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    }

    // Load font
    ghastly_font = TTF_OpenFont("assets/fonts/ghastly.ttf", 28);


    if (!ghastly_font) {
        std::cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }

    // Initialize buttons
    playButton = Button(renderer, "Play", ghastly_font, { 100, 200, 200, 50 });
    quitButton = Button(renderer, "Quit", ghastly_font, {100, 400, 200, 50});
}




void MainMenu::handleEvent(SDL_Event& e) {
    // Handle events for buttons
    playButton.handleEvent(e);
    quitButton.handleEvent(e);

    // Check if buttons are clicked
    if (playButton.isClicked()) {
        gameStarted = true; // Set game started flag
    }
    else if (quitButton.isClicked()) {
        gameStarted = false; // Set game started flag to indicate a quit
    }
}

void MainMenu::render() {
    // Draw background
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
    }

    // Render buttons
    playButton.render();
    quitButton.render();
}

bool MainMenu::isGameStarted() const {
    return gameStarted;
}

bool MainMenu::shouldQuit() const {
    return quitButton.isClicked();
}

MainMenu::~MainMenu() {
    if (backgroundTexture != nullptr) {
        SDL_DestroyTexture(backgroundTexture);
    }
    
    if (ghastly_font != nullptr) {
        TTF_CloseFont(ghastly_font);
        ghastly_font = nullptr;
    }
}
