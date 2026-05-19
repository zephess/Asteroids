
#pragma once
#include "Game.h"
#include "Player.h"
#include "Projectile.h"


#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
const int SCREENWIDTH = 1920;
const int SCREENHEIGHT = 1080;
Game game;
Player::Player(SDL_Renderer* renderer) 
	:posX{ 0 }, posY{ 0 }, velX{ 0 }, velY{ 0 }, renderer{ renderer }, body{ 50,SCREENHEIGHT/2,80,80 }, bodyTxt{ nullptr }, speed{ 7 } {
	//initialise surface
	SDL_Surface* srf = IMG_Load("ship2.png");
	if (!srf) {
		std::cout << "Failed to load player image";
		return;
	}
	//initialise texture
	bodyTxt = SDL_CreateTextureFromSurface(renderer, srf);
	SDL_FreeSurface(srf);
	if (!bodyTxt) {
		std::cerr << "Failed to create texture from player image" << SDL_GetError();
		return;
	}
	
}

Player::~Player() {

}


void Player::EventHandler(const SDL_Event &evt) {
	
	//handle inputs
	//key down inputs
	if (evt.type == SDL_KEYDOWN && evt.key.repeat == 0) {
		switch (evt.key.keysym.sym) {
		case SDLK_w:
			velY -= speed;
			break;
		case SDLK_s:
			velY += speed;
			break;
		case SDLK_d:
			velX += speed;
			break;
		case SDLK_a:
			velX -= speed;
			break;
		
		}
	}
	//key up inputs
	else if (evt.type == SDL_KEYUP && evt.key.repeat == 0) {
		switch (evt.key.keysym.sym) {
		case SDLK_w:
			velY += speed;
			break;
		case SDLK_s:
			velY -= speed;
			break;
		case SDLK_d:		
			velX -= speed;
			break;
		case SDLK_a:		
			velX += speed;
			break;
		}		
	}
}

void Player::render() {
	if (bodyTxt) {
		//move
		body.x += velX;
		body.y += velY;
		//render
		SDL_RenderCopy(renderer, bodyTxt, nullptr, &body);
	}
}

SDL_Rect* Player::getBody() {
	//return player rect
	return &body;
}

void Player::keepInBounds() {
	//if out of bounds, move back in
	if (body.x < 0) {
		body.x = 1;
	}
	if (body.x + body.w > SCREENWIDTH) {
		body.x = SCREENWIDTH - 1 - body.w;
	}
	if (body.y < 0) {
		body.y = 1;
	}
	if (body.y + body.h > SCREENHEIGHT) {
		body.y = SCREENHEIGHT - 1 - body.h;
	}
}