#include "Game.h"
#include "Asteroids.h"
#include <SDL.h>
#include <iostream>
#include <random>
#include <SDL_image.h>
const int SCREENWIDTH = 1920;
const int SCREENHEIGHT = 1080;
Asteroids::Asteroids(SDL_Renderer* renderer, float speedOffset)
	:renderer(renderer), y{ 0 }, width{ 0 }, height{ 0 }, body{0,0,0,0}, speed{ 2 }, bodyTxt{ nullptr } {
	//set random seed
	srand(SDL_GetTicks());
	
	//initialise surface
	SDL_Surface* srf = IMG_Load("asteroid.png");
	if (!srf) {
		std::cout << "Failed to load player image";
		return;
	}

	//set texture
	bodyTxt = SDL_CreateTextureFromSurface(renderer, srf);
	//free surface
	SDL_FreeSurface(srf);
	
	//set random speed based on speedOffset
	speed = speedOffset + rand() % 5;
	//set random height to spawn at
	y = rand() % (((SCREENHEIGHT-300) - 100) +1) +100;
	//set random dimensions
	width = 100+rand()%200;
	height = width;
	//apply
	body = { SCREENWIDTH,y, width, height};
}

Asteroids::~Asteroids() {
	
}

void Asteroids::render() {
	//render
	SDL_RenderCopy(renderer, bodyTxt, nullptr, &body);
}

void Asteroids::move() {
	//move
	body.x -= speed;
}

SDL_Rect* Asteroids::getBody() {
	//return the asteroids rect
	return &body;
}