#pragma once
#include <SDL.h>

#include "Game.h"
#include "Projectile.h"
#include <iostream>

class Player {
private:
	int velX;
	int velY;
	int posX;
	int posY;
	SDL_Rect body;
	SDL_Texture* bodyTxt;
	SDL_Renderer* renderer;
	int speed;
	
public:
	Player(SDL_Renderer* renderer);
	~Player();
	void EventHandler(const SDL_Event &evt);
	void render();
	SDL_Rect* getBody();
	void keepInBounds();
};