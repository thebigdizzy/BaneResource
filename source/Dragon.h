/*
 * Dragon.h
 *
 *  Created on: Aug 3, 2016
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
#include <vector>

using namespace std;

class Dragon {
public:
	bool active;
	bool dropIn;
	SDL_Rect posRect;
	SDL_Texture *texture;
	float posX, posY;

	Dragon(){}

	Dragon(SDL_Renderer *renderer, string filePath, string audioPath, int x, int y){
		active = true;

		dropIn = false;

		posRect.x = posX = x;
		posRect.y = posY = y;

		string path = filePath + "Dragon.png";

		texture = IMG_LoadTexture(renderer, path.c_str());
		int w, h;
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		posRect.w = w/3;
		posRect.h = h/3;
	}

	void Draw(SDL_Renderer *renderer){
		SDL_RenderCopy(renderer, texture, NULL, &posRect);
	}

	void DropIn(float deltaTime){
		if(posRect.y < 100){
			posY += 200 *deltaTime;
			posRect.y = (int)(posY + .5f);
		}
		if(posRect.y >= 100){
			posY = 100;
			posRect.y = (int)posY;
		}
	}

	void Update(float deltaTime){
		if(dropIn){
			DropIn(deltaTime);
		}
	}
};
