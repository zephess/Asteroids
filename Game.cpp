
#include <SDL.h>
#include <iostream>
#include "Game.h"
#include "Asteroids.h"
#include "Player.h"
#include "Projectile.h"
#include <vector>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
SDL_Event evt;
const int SCREENWIDTH = 1920;
const int SCREENHEIGHT = 1080;
Game::Game()
	:renderer{ nullptr }, backgroundTxt{ nullptr }, bgMusic{nullptr}, gameOver{nullptr}, livesRect{0,0,0,0},livesTxt{nullptr}, playerExplosion{nullptr}, scoreRect{0,0,0,0}, scoreTxt{nullptr}, instructionsTxt{nullptr},
	window{nullptr}, txtSurface{nullptr}, background{nullptr}, ExitFlag{false}, inMenu{false}, player{nullptr}, lives{3}, score{0}, quitTxt{nullptr}, playTxt{nullptr}, font{nullptr}, howToTxt{nullptr},
	speedOffset{ 1 }, asts{ 100000 }, flip{ true }, astOnScreen{ 0 }, laser{ nullptr }, explosion{ nullptr }, astCount{ 0 }, projs{ 10000 }, i{ 0 }, playButton{ 0,0,0,0 }, quitButton{ 0,0,0,0 }, titleTxt{ nullptr }, instructionsButton{ 0,0,0,0 } {
	
}

Game::~Game() {
	SDL_FreeSurface(txtSurface);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

int Game::Init(){

	//initialising
	SDL_Init(SDL_INIT_EVERYTHING);
	
	//creating window
	window = SDL_CreateWindow("Asteroids (but cooler)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,SCREENWIDTH, SCREENHEIGHT, SDL_WINDOW_RESIZABLE);
	if (window == nullptr) {
		std::cout << "Error creating window" << std::endl;
		return 1;
	}
	
	else {
		SDL_Surface* surface = SDL_GetWindowSurface(window);
	}
	
	//creating renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		std::cout << "Error creating renderer" << std::endl;
		SDL_DestroyWindow(window);
		return 2;
	}
	
	//initialise background image
	background = IMG_Load("background2.png");
	if (background == nullptr) {
		std::cout << "Error creating surface" << SDL_GetError();
	}
	
	//create background texture
	else {
		backgroundTxt = SDL_CreateTextureFromSurface(renderer, background);
	}
	
	if (backgroundTxt == nullptr) {
		std::cout << "Error creating texture from surface" << SDL_GetError();
	}
	
	//initialise audio
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << "Error initialising SDL Mixer" << SDL_GetError();
		return 3;
	}
	
	//load background music
	bgMusic = Mix_LoadMUS("bgLoop.wav");
	
	if (!bgMusic) {
		std::cout << "error loading music" << SDL_GetError();
	}
	
	//load sfx
	laser = Mix_LoadWAV("laser.wav");
	explosion = Mix_LoadWAV("explosion.wav");
	playerExplosion = Mix_LoadWAV("playerExplosion.wav");
	gameOver = Mix_LoadWAV("gameOver.wav");
	
	//set volumes
	Mix_Volume(1, 15);
	Mix_Volume(2, 3);
	Mix_Volume(3, 15);
	
	//free surface
	SDL_FreeSurface(background);

	//initialise fonts
	if (TTF_Init() == -1) {
		std::cout << "Error Initialising SDL_ttf" << SDL_GetError();
		return 4;
	}

	//create font
	font = TTF_OpenFont("PressStart2P-vaV7.ttf", 16);
	//create font surface
	SDL_Surface* txtSurface = TTF_RenderText_Solid(font, "Play", SDL_Color{ 255,255,255});
	if (!txtSurface) {
		std::cout << "Error rendering text surface" << SDL_GetError();
	}
	//create texture from surface
	playTxt = SDL_CreateTextureFromSurface(renderer, txtSurface);
	if (!playTxt) {
		std::cout << "Error rendering text texture" << SDL_GetError();
	}

	//free surface
	SDL_FreeSurface(txtSurface);

	//ditto
	txtSurface = TTF_RenderText_Solid(font, "Quit", SDL_Color{ 255,255,255 });
	if (!txtSurface) {
		std::cout << "Error rendering text surface" << SDL_GetError();
	}

	quitTxt = SDL_CreateTextureFromSurface(renderer, txtSurface);
	SDL_FreeSurface(txtSurface);
	txtSurface = TTF_RenderText_Solid(font, "Asteroids (But Cooler)", SDL_Color{ 255,255,255 });
	if (!txtSurface) {
		std::cout << "Error rendering text surface" << SDL_GetError();
	}
	titleTxt = SDL_CreateTextureFromSurface(renderer, txtSurface);
	SDL_FreeSurface(txtSurface);

	txtSurface = TTF_RenderText_Solid(font, "Instructions", SDL_Color{ 255,255,255 });
	if (!txtSurface) {
		std::cout << "Error rendering text surface" << SDL_GetError();
	}
	instructionsTxt = SDL_CreateTextureFromSurface(renderer, txtSurface);
	SDL_FreeSurface(txtSurface);

	std::string TEXT = "How to play\n\n"
	"WASD: Move ship\n"					
	"SPACE: Shoot\n\n"
	"Prevent the asteroids from reaching the edge of the screen!"
	"\n\n\nPress ESCAPE to continue";
	txtSurface = TTF_RenderText_Blended_Wrapped(font, TEXT.c_str(), SDL_Color{255,255,255}, 380);
	if (!txtSurface) {
		std::cout << "Error rendering text surface" << SDL_GetError();
	}
	howToTxt = SDL_CreateTextureFromSurface(renderer, txtSurface);
	SDL_FreeSurface(txtSurface);
	//set UI rects
	livesRect = { 0,0,50,50 };
	scoreRect = { SCREENWIDTH - 100, 0, 100,50 };
	
	//load menu screem
	inMenu = true;
	MenuScreen();
	return 0;
}

void Game::EventHandler() {
	//while any event is happening
	while (SDL_PollEvent(&evt) != 0) {
		//while in game
		if (!inMenu) {
			//run player event handler (covers movement)
			player->EventHandler(evt);
			if (evt.type == SDL_KEYDOWN && evt.key.repeat == 0) {
				switch (evt.key.keysym.sym) {
				//if space pressed, spawn projectile
				case SDLK_SPACE:
					//play sfx
					Mix_PlayChannel(1, laser, 0);
					SpawnProjectile();
					break;
				//if escape pressed, break gameLoop
				case SDLK_ESCAPE:
					ExitFlag = true;
					break;
				}
			//if window closed, break gameLoop
				if (evt.type == SDL_QUIT) {
					ExitFlag = true;
					SDL_Quit();
				}
			}
		}
		//while in menu
		if (inMenu) {
			//if hovering over play button
			if (MouseOverButton(playButton)) {
				//if leftmouse clicked
				if (evt.button.button == SDL_BUTTON_LEFT) {
					if (!inStructions) {
						//break out of menu
						inMenu = false;
					}
					
				}
			}
			//if hovering over quit button
			if (MouseOverButton(quitButton)) {
				//if leftmouse clicked
				if (evt.button.button == SDL_BUTTON_LEFT) {
					//quit
					SDL_Quit();
					return;
				}
			}
			if (MouseOverButton(instructionsButton)) {
				//if leftmouse clicked
				if (evt.button.button == SDL_BUTTON_LEFT) {
					inStructions = true;
				}
			}
			if (inStructions) {
				if (evt.type == SDL_KEYDOWN && evt.key.repeat == 0) {
					if (evt.key.keysym.sym == SDLK_ESCAPE) {
						inStructions = false;
					}
				}
			}
			
		}
	}
	return;
}

void Game::GameLoop(SDL_Rect body) {
	Uint32 tickCount = 0;
	//while user has not exited and user has not died
	while (!ExitFlag  && lives >0) {
		while (!SDL_TICKS_PASSED(SDL_GetTicks(), tickCount + 16));
		float deltaTime = (SDL_GetTicks() - tickCount) / 1000.0f;// time since last frame calculated
		// clamp the maximum delta time
		if (deltaTime > 0.05f)
		{
			deltaTime = 0.05f;
		}
		tickCount = SDL_GetTicks();// update tick count for next frame
		
		
		//process events
		EventHandler();
		//process updates
		UpdateGame();
		//resets draw colour
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		//makes sure player is onscreen
		player->keepInBounds();
		
	}
	//if dead, plays game over sfx
	if (lives <=0) {
		Mix_PlayChannel(4, gameOver, 0);
	}
	//stops background music
	Mix_HaltMusic();
	//reinitialises game variables
	reInitVariables();
	//returns to menu
	inMenu = true;
	MenuScreen();
	return;
}
//CORE GAME UPDATES
void Game::UpdateGame() {
	//clears screen
	SDL_RenderClear(renderer);
	//if not playing music, play music
	if (Mix_PlayingMusic() == 0) {
		Mix_VolumeMusic(7);
		Mix_PlayMusic(bgMusic, -1);
	}
	//if player is initialised, render it
	if (player) {
		player->render();
	}
	//UI UPDATES
	//renders life count in top left
	std::string lvs = std::to_string(lives);
	txtSurface = TTF_RenderText_Solid(font, lvs.c_str(), SDL_Color{255,255,255});
	livesTxt = SDL_CreateTextureFromSurface(renderer, txtSurface);
	SDL_RenderCopy(renderer, livesTxt, nullptr, &livesRect);
	SDL_FreeSurface(txtSurface);
	//renders score count in top right
	std::string scr = std::to_string(score);
	txtSurface = TTF_RenderText_Solid(font, scr.c_str(), SDL_Color{ 255,255,255 });
	scoreTxt = SDL_CreateTextureFromSurface(renderer, txtSurface);
	SDL_RenderCopy(renderer, scoreTxt, nullptr, &scoreRect);
	
	//if projectiles exist
	if (!projs.empty()) {
		//for each projectile that exists
		for (int j = 0; j < i; j++) {
			//if the projectile is currently alive
			if (!(projs.at(j) == nullptr)) {
				//render it
				projs.at(j)->render();
				//move it
				projs.at(j)->move();
			}
		}
	}

	//if asteroids exist
	if (!asts.empty()) {
		//for each asteroid that exists
		for (int j = 0; j < astCount; j++) {
			//if the asteroid is currently alive
			if (!(asts.at(j) == nullptr)) {
				//render it
				asts.at(j)->render();
				//move it
				asts.at(j)->move();
				//if that asteroid is colliding with the player
				if (asts.at(j)->getBody()->x < 0 || SDL_HasIntersection(asts.at(j)->getBody(), player->getBody())) {
					//destroy it
					asts.at(j) = nullptr;
					//play sfx
					Mix_PlayChannel(3, playerExplosion, 0);
					//decrease onScreen variable
					astOnScreen--;
					//decrease player lives
					lives--;
				}
				//if projectiles exist
				if (!projs.empty()) {
					//for each projectile that exists
					for (int k = 0; k < i;k++) {
						//if the projectile is currently alive
						if (projs.at(k) != nullptr) {
							//check it for collisions with each asteroid that is currently alive
							if (SDL_HasIntersection(projs.at(k)->getBody(), asts.at(j)->getBody())) {
								//then destroy the asteroid
								asts.at(j) = nullptr;
								//and the projectile that hit it
								projs.at(k) = nullptr;
								//play sfx
								Mix_PlayChannel(2, explosion, 0);
								//decrease onScreen variable
								astOnScreen--;
								//add score
								score += 10;								
							}
						}
					}
				}
			}

		}
	}
	//if number of asteroids currently alive < 5
	if (astOnScreen < 5) {
		//spawn another
		SpawnAsteroids();
	}
	//update renderer
	SDL_RenderPresent(renderer);
	return;
}

void Game::Quit() {
	//frees memory
	SDL_FreeSurface(txtSurface);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return;
}

void Game::SpawnProjectile() {
	//for visual effect, alternates where projectile shoots from
	//creates projectile, adds to projectile vector
	if (flip) {
		//create projectile from top gun(visual)
		projs.at(i) = new Projectile(renderer, player->getBody()->x + player->getBody()->w, player->getBody()->y);
		flip = false;
	}
	else {
		//create projectile from bottom gun(visual)
		projs.at(i) = new Projectile(renderer, player->getBody()->x + player->getBody()->w, player->getBody()->y + (player->getBody()->h / 2));
		flip = true;
	}
	//increments (tracks how many projectiles have been spawned
	i++;
	return;
}
void Game::SpawnAsteroids() {
	//increments speed of asteroids progressively
	if (astCount % 10 == 0) {
		speedOffset+= 0.5f;
	}
	//creates new asteroid and adds to asteroid vector
	asts.at(astCount) = new Asteroids(renderer, speedOffset);
	//tracks total amount of asteroids spawned
	astCount++;
	//tracks amount of asteroids on screen
	astOnScreen++;
	return;

}
void Game::MenuScreen() {
	
	//create buttons
	playButton = { SCREENWIDTH/2-150,SCREENHEIGHT/2-200,300,125 };
	//extra rect for text formatting
	SDL_Rect textRect1{ SCREENWIDTH / 2 - 130,SCREENHEIGHT / 2-180, 270, 95 };
	//ditto
	quitButton = { SCREENWIDTH / 2 - 150,SCREENHEIGHT / 2 +200,300,125 };
	SDL_Rect textRect2{ SCREENWIDTH / 2 - 130,SCREENHEIGHT / 2 +220, 270, 95 };
	
	instructionsButton = { SCREENWIDTH / 2 - 150, SCREENHEIGHT / 2, 300,125 };
	SDL_Rect textRect3{ SCREENWIDTH / 2 - 150, SCREENHEIGHT / 2 +20, 300,95 };
	
	SDL_Rect titleRect{ SCREENWIDTH / 2 - 400, SCREENHEIGHT / 4 -100, 800, 95 };

	SDL_Rect instructionsRect{ SCREENWIDTH / 8, SCREENHEIGHT / 8, (SCREENWIDTH / 4) * 3, (SCREENHEIGHT / 4) * 3 };
	SDL_Rect textRect4{ SCREENWIDTH / 8 + 20, SCREENHEIGHT / 8 + 20,(SCREENWIDTH / 4) * 3, (SCREENHEIGHT / 4) * 3 -20 };
	
	while (inMenu) {
		//render menu
		SDL_Rect screen{ 0,0,SCREENWIDTH,SCREENHEIGHT };

		SDL_RenderCopy(renderer, backgroundTxt, nullptr, &screen);
		SDL_SetRenderDrawColor(renderer, 159, 159, 159, 255);
		SDL_RenderFillRect(renderer, &playButton);
		SDL_RenderFillRect(renderer, &instructionsButton);
		SDL_RenderFillRect(renderer, &quitButton);
		SDL_RenderCopy(renderer, instructionsTxt, nullptr, &textRect3);
		SDL_RenderCopy(renderer, playTxt, nullptr, &textRect1);
		SDL_RenderCopy(renderer, quitTxt, nullptr, &textRect2);
		SDL_RenderCopy(renderer, titleTxt, nullptr, &titleRect);
		SDL_RenderPresent(renderer);
		//call event handler
		EventHandler();
		while (inStructions) {
			std::string TEXT = "How to play\nWASD: Move ship\SPACE: Shoot\nPrevent the asteroids from reaching the edge of the screen!\n\n\nPress ESCAPE to continue";
			SDL_RenderFillRect(renderer, &instructionsRect);
			//TTF_RenderUTF8_Solid(font, TEXT.c_str(), SDL_Color{255,255,255});
			SDL_RenderCopy(renderer, howToTxt, nullptr, &textRect4);
			SDL_RenderPresent(renderer);
			EventHandler();
		}
	}
	//once out of menu, initialise player and start gameLoop
	ExitFlag = false;
	player = new Player(renderer);
	GameLoop(*player->getBody());
	return;

}

bool Game::MouseOverButton(SDL_Rect button) {
	//checks mouse position in relation to rect parameter
	if (evt.motion.x < button.x) {
		return false;
	}
	if (evt.motion.x > button.x + button.w) {
		return false;
	}
	if (evt.motion.y < button.y) {
		return false;
	}
	if (evt.motion.y > button.y + button.h) {
		return false;
	}
	return true;
}

void Game::reInitVariables() {
	//reinitialises important game variables to reset game
	astCount = 0;
	astOnScreen = 0;
	lives = 3;
	score = 0;
	speedOffset = 1;
	return;
}