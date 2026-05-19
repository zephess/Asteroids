#pragma once
#include "Game.h"
#include <SDL.h>
#include <iostream>

class Asteroids {
private:
	SDL_Rect body;
	int y, height, width, speed;
	SDL_Renderer* renderer;
	SDL_Texture* bodyTxt;

public:
	Asteroids(SDL_Renderer* renderer, float speedOffset);
	~Asteroids();
	void render();
	void move();
	SDL_Rect* getBody();
};