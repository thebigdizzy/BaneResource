/*
 * pterodactyl.h
 *
 *  Created on: Aug 8, 2016
 *      Author: danieltrayler
 */


#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
//#include "SDL2_mixer/SDL_mixer.h"
//#include "SDL2_ttf/SDL_ttf.h"
#endif

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
//#include "SDL_mixer.h"
//#include "SDL_ttf.h"
#endif

#include <stdio.h>
#include <iostream>

using namespace std;

class Pterodactyl{
public:
	bool active;

	float posX, posY, start, sX, sY;

	bool begin;
	int health;

	SDL_Rect posRect;
	SDL_Texture *texture;

	enum State {Idle, Dive, Chase, Return, Die};

	State state;

	Pterodactyl(){}

	Pterodactyl(SDL_Renderer *renderer, string filePath, string audioPath, int x, int y){
		active = true;
		posRect.x = posX = sX = x;
		posRect.y = posY = sY = y;

		health = 100;
		begin = true;
		start = 0;

		string path = filePath + "pterodactyl.png";
		texture = IMG_LoadTexture(renderer, path.c_str());

		if(texture == NULL){
			cout << "texture not found" << endl;
		}

		int w, h;
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		posRect.w = w/10;
		posRect.h = h/10;

		state = Idle;
	}

	void Draw(SDL_Renderer *renderer){
		SDL_RenderCopy(renderer, texture, NULL, &posRect);
	}

	void Diving(float deltaTime, SDL_Rect playerRect){
		posY += 200 * deltaTime;
		posRect.y = (int)(posY + .5f);

		posX -= 100 * deltaTime;
		posRect.x = (int)(posX + .5f);

		if(posRect.y >= playerRect.y){
			state = Chase;
		}
	}

	void Dash(float deltaTime, SDL_Rect playerRect){
		if(begin){
			int distance = playerRect.x - posRect.x;
			start = posX + distance;
			begin = false;
		}

		posX -= 200 * deltaTime;
		posRect.x = (int)(posX + .5f);

		if(posRect.x <= start){
			state = Return;
			begin = true;
		}
	}

	void FlyUp(float deltaTime){
		if(posRect.y >= -100){
			posY -= 200 * deltaTime;
			posRect.y = (int)(posY + .5f);

			posX -= 100 * deltaTime;
			posRect.x = (int)(posX + .5f);
		} else {
			state = Idle;
		}
	}

	void MoveY(float playerSpeed, float deltaTime){
		posY += playerSpeed * deltaTime;

		posRect.y = (int)(posY + .5f);
	}

	void MoveX(float playerSpeed, float deltaTime){
		posX += playerSpeed * deltaTime;

		posRect.x = (int)(posX + .5f);
	}

	void Update(float deltaTime, SDL_Rect PlayerRect){
		switch(state){
		case Idle:
			active = false;
			posX = sX;
			posY = sY;
			posRect.x = (int)(posX + .5f);
			posRect.y = (int)(posY + .5f);
			break;
		case Dive:
			active = true;
			Diving(deltaTime, PlayerRect);
			break;
		case Chase:
			Dash(deltaTime, PlayerRect);
			break;
		case Return:
			FlyUp(deltaTime);
			break;
		case Die:

			break;
		default:break;
		}
	}
};