#pragma once
#include <SDL.h>
#include <iostream>
#include "Asteroids.h"
#include "Player.h"
#include "Projectile.h"
#include <vector>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
class Game {
private:
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Surface* txtSurface, *background;
	SDL_Texture* backgroundTxt, *quitTxt, *playTxt, *instructionsTxt, *howToTxt, *livesTxt, *scoreTxt, *titleTxt;
	Player* player;
	bool flip;
	std::vector<Projectile*> projs;
	int i, score, astCount, astOnScreen, lives;
	float speedOffset;
	bool ExitFlag, inMenu, inStructions;
	SDL_Rect playButton, quitButton, instructionsButton, livesRect, scoreRect;
	std::vector<Asteroids*> asts;
	Mix_Chunk* laser, *explosion, *playerExplosion, *gameOver;
	Mix_Music* bgMusic;
	TTF_Font* font;

public: 
	Game();
	~Game();
	int Init();
	void EventHandler();
	void GameLoop(SDL_Rect body);
	void Quit();
	void UpdateGame();
	void SpawnProjectile();
	void SpawnAsteroids();
	void MenuScreen();
	bool MouseOverButton(SDL_Rect button);
	void reInitVariables();

	
};