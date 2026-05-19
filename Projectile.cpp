
#include <SDL.h>
#include "Game.h"
#include "Projectile.h"
#include <iostream>
#include <SDL_image.h>
Projectile::Projectile(SDL_Renderer* renderer, int x, int y)
	:renderer{ renderer }, body{ x,y,30,30 }, bodyTxt{ nullptr } {
	//initialise surface
	SDL_Surface* srf = IMG_Load("bullet.png");
	if (!srf) {
		std::cout << "Failed to load image";
		return;
	}
	//initialise texture
	bodyTxt = SDL_CreateTextureFromSurface(renderer, srf);
	SDL_FreeSurface(srf);
	if (!bodyTxt) {
		std::cerr << "Failed to create texture from image" << SDL_GetError();
		return;
	}
}
Projectile::~Projectile() {

}

void Projectile::render() {
	if (bodyTxt) {
		//render
		SDL_RenderCopy(renderer, bodyTxt, nullptr, &body);
	}
}
void Projectile::move() {
	//move
	body.x += 20;
}

SDL_Rect* Projectile::getBody() {
	//return projectile rect
	return &body;
}




