#pragma once
#include "Game.h"
#include <SDL.h>
#include <iostream>

class Projectile{
private:
	SDL_Renderer* renderer;
	SDL_Rect body;
	SDL_Texture* bodyTxt;
public:
	Projectile(SDL_Renderer* renderer, int x, int y);
	~Projectile();
	void render();
	void move();
	SDL_Rect* getBody();
};